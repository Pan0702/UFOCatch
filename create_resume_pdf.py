#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
履歴書用技術アピール資料PDF生成スクリプト
UFOCatchゲームプロジェクトの技術的特徴をまとめたドキュメント
"""

from reportlab.lib.pagesizes import A4
from reportlab.lib.styles import getSampleStyleSheet, ParagraphStyle
from reportlab.lib.units import mm
from reportlab.platypus import SimpleDocTemplate, Paragraph, Spacer, PageBreak, Preformatted
from reportlab.lib.enums import TA_LEFT, TA_CENTER
from reportlab.pdfbase import pdfmetrics
from reportlab.pdfbase.ttfonts import TTFont
from reportlab.lib.colors import HexColor
import os

def create_technical_resume_pdf():
    """技術アピール資料PDFを生成"""

    # PDFファイルの設定
    pdf_filename = "UFOCatch_技術アピール資料.pdf"
    doc = SimpleDocTemplate(
        pdf_filename,
        pagesize=A4,
        leftMargin=20*mm,
        rightMargin=20*mm,
        topMargin=20*mm,
        bottomMargin=20*mm
    )

    # 日本語フォントの登録を試みる
    font_registered = False
    japanese_fonts = [
        '/usr/share/fonts/truetype/noto/NotoSansCJK-Regular.ttc',
        '/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc',
        '/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf',
    ]

    for font_path in japanese_fonts:
        if os.path.exists(font_path):
            try:
                pdfmetrics.registerFont(TTFont('Japanese', font_path))
                font_registered = True
                break
            except:
                continue

    # スタイル定義
    styles = getSampleStyleSheet()

    # カスタムスタイル
    title_style = ParagraphStyle(
        'CustomTitle',
        parent=styles['Heading1'],
        fontSize=24,
        textColor=HexColor('#1a237e'),
        spaceAfter=30,
        alignment=TA_CENTER,
        fontName='Japanese' if font_registered else 'Helvetica-Bold'
    )

    heading1_style = ParagraphStyle(
        'CustomHeading1',
        parent=styles['Heading1'],
        fontSize=18,
        textColor=HexColor('#283593'),
        spaceAfter=12,
        spaceBefore=20,
        fontName='Japanese' if font_registered else 'Helvetica-Bold'
    )

    heading2_style = ParagraphStyle(
        'CustomHeading2',
        parent=styles['Heading2'],
        fontSize=14,
        textColor=HexColor('#3949ab'),
        spaceAfter=10,
        spaceBefore=15,
        fontName='Japanese' if font_registered else 'Helvetica-Bold'
    )

    body_style = ParagraphStyle(
        'CustomBody',
        parent=styles['Normal'],
        fontSize=10,
        leading=16,
        fontName='Japanese' if font_registered else 'Helvetica'
    )

    code_style = ParagraphStyle(
        'CodeStyle',
        parent=styles['Code'],
        fontSize=8,
        leading=12,
        leftIndent=10,
        fontName='Courier',
        backColor=HexColor('#f5f5f5')
    )

    # ドキュメント要素のリスト
    story = []

    # タイトルページ
    story.append(Spacer(1, 40*mm))
    story.append(Paragraph("UFOCatch Game Project", title_style))
    story.append(Spacer(1, 10*mm))
    story.append(Paragraph("Technical Skills Portfolio", heading1_style))
    story.append(Spacer(1, 5*mm))
    story.append(Paragraph("DirectX & C++ Game Development", body_style))
    story.append(PageBreak())

    # プロジェクト概要
    story.append(Paragraph("Project Overview", heading1_style))
    story.append(Spacer(1, 5*mm))

    overview_text = """
    <b>Game Concept:</b> Stealth action game where a UFO abducts animals while avoiding detection by humans.<br/>
    <br/>
    <b>Technology Stack:</b><br/>
    - Language: C++<br/>
    - Graphics API: DirectX 11<br/>
    - Architecture: Component-based game engine<br/>
    - Design Patterns: State Pattern, Object Pool, Singleton<br/>
    <br/>
    <b>Development Period:</b> Personal project<br/>
    <b>Lines of Code:</b> ~10,000+ lines of custom code
    """
    story.append(Paragraph(overview_text, body_style))
    story.append(Spacer(1, 10*mm))

    # 技術アピールポイント
    story.append(PageBreak())
    story.append(Paragraph("5 Key Technical Achievements", heading1_style))
    story.append(Spacer(1, 10*mm))

    # 1. 空間分割アルゴリズム
    story.append(Paragraph("1. Spatial Partitioning Algorithm (Linear Quadtree + Morton Order)", heading2_style))
    story.append(Paragraph("<b>Location:</b> Src/09_Enemy/Liner4Tree.h", body_style))
    story.append(Spacer(1, 3*mm))

    tech1_desc = """
    <b>Implementation:</b><br/>
    - Quadtree-based collision detection optimization<br/>
    - Morton curve (Z-order curve) for efficient spatial indexing using bit operations<br/>
    - Reduces collision check complexity from O(n²) to O(n log n)<br/>
    <br/>
    <b>Technical Highlights:</b><br/>
    - Implemented bit separation algorithm for Morton encoding<br/>
    - Hierarchical cell traversal for spatial queries<br/>
    - Template-based generic container for any game object type
    """
    story.append(Paragraph(tech1_desc, body_style))
    story.append(Spacer(1, 3*mm))

    code1 = """// Morton number calculation with bit separation
uint16_t BitSeparate(uint16_t n) {
    n = (n | (n << 8)) & 0x00ff00ff;
    n = (n | (n << 4)) & 0x0f0f0f0f;
    n = (n | (n << 2)) & 0x33333333;
    n = (n | (n << 1)) & 0x55555555;
    return n;
}

uint16_t Get2DMortonNumber(float worldX, float worldY) {
    uint16_t cellX = normalizeCoord(worldX);
    uint16_t cellY = normalizeCoord(worldY);
    return BitSeparate(cellX) | (BitSeparate(cellY) << 1);
}"""
    story.append(Preformatted(code1, code_style))
    story.append(Spacer(1, 10*mm))

    # 2. 幾何学的衝突判定
    story.append(PageBreak())
    story.append(Paragraph("2. Geometric Collision Detection System", heading2_style))
    story.append(Paragraph("<b>Location:</b> Src/11_GameSystem/VisionSystem.h/cpp", body_style))
    story.append(Spacer(1, 3*mm))

    tech2_desc = """
    <b>Implementation:</b><br/>
    - Advanced sector-circle collision detection for AI vision system<br/>
    - Line segment-circle intersection using quadratic equation discriminant<br/>
    - Applied to enemy AI detection mechanics<br/>
    <br/>
    <b>Technical Highlights:</b><br/>
    - Mathematical approach using dot products and vector normalization<br/>
    - Optimized with early rejection tests to minimize calculations<br/>
    - Handles edge cases including sector boundaries and arc intersections
    """
    story.append(Paragraph(tech2_desc, body_style))
    story.append(Spacer(1, 3*mm))

    code2 = """// Line segment-circle intersection detection
bool LineSegmentCircleIntersection(
    const VECTOR2& lineStart, const VECTOR2& lineEnd,
    const VECTOR2& circleCenter, float circleRadius) const {

    VECTOR2 direction = lineEnd - lineStart;
    VECTOR2 toCenter = lineStart - circleCenter;

    float a = dot(direction, direction);
    float b = 2.0f * dot(toCenter, direction);
    float c = dot(toCenter, toCenter) - radius * radius;

    float discriminant = b * b - 4.0f * a * c;
    if (discriminant < 0.0f) return false;

    float t1 = (-b - sqrt(discriminant)) / (2.0f * a);
    float t2 = (-b + sqrt(discriminant)) / (2.0f * a);

    return (t1 >= 0 && t1 <= 1) || (t2 >= 0 && t2 <= 1);
}"""
    story.append(Preformatted(code2, code_style))
    story.append(Spacer(1, 10*mm))

    # 3. ステートパターン
    story.append(PageBreak())
    story.append(Paragraph("3. Design Pattern Implementation (State Pattern)", heading2_style))
    story.append(Paragraph("<b>Location:</b> Src/12_Tutorial/TutorialState.h, Src/09_Enemy/Human/State/", body_style))
    story.append(Spacer(1, 3*mm))

    tech3_desc = """
    <b>Implementation:</b><br/>
    - State machine for complex behavior management<br/>
    - Tutorial progression: Move → Suction → Expand → Discovery → Play<br/>
    - Enemy AI states: Idle → Walk → FindPlayer<br/>
    <br/>
    <b>Technical Highlights:</b><br/>
    - Separation of concerns: Actor and State classes<br/>
    - std::unordered_map for O(1) state lookup<br/>
    - Pre-allocated states to eliminate runtime allocations<br/>
    - Prevents frame rate drops during state transitions
    """
    story.append(Paragraph(tech3_desc, body_style))
    story.append(Spacer(1, 3*mm))

    code3 = """// State pattern implementation
class CTutorialState {
public:
    virtual void Enter() {}
    virtual void Update() {}
    virtual void Exit() {}

    enum class State {
        Move, Suction, Expands, Discovery, Play
    };
};

class CMoveState : public CTutorialState {
    void Update() override {
        // Handle movement tutorial logic
        if (playerMoved()) transitionToNext();
    }
};

// State management with unordered_map
std::unordered_map<StateType, std::unique_ptr<State>> states;"""
    story.append(Preformatted(code3, code_style))
    story.append(Spacer(1, 10*mm))

    # 4. DirectX HLSL
    story.append(PageBreak())
    story.append(Paragraph("4. DirectX HLSL Shader Programming", heading2_style))
    story.append(Paragraph("<b>Location:</b> Hlsl/DisplaceSkinMesh.hlsli", body_style))
    story.append(Spacer(1, 3*mm))

    tech4_desc = """
    <b>Implementation:</b><br/>
    - Displacement mapping for advanced surface detail<br/>
    - Skinned mesh animation supporting up to 255 bones<br/>
    - Normal mapping, specular mapping for realistic rendering<br/>
    <br/>
    <b>Technical Highlights:</b><br/>
    - Hull and Domain shaders for tessellation<br/>
    - Vertex skinning with bone weight blending<br/>
    - Tangent space calculations for normal mapping<br/>
    - Optimized constant buffer usage
    """
    story.append(Paragraph(tech4_desc, body_style))
    story.append(Spacer(1, 3*mm))

    code4 = """// Skinning vertex shader
struct VS_IN {
    float3 Pos : POSITION;
    float3 Normal : NORMAL;
    float2 Tex : TEXCOORD;
    uint4 Bones : BONE_INDEX;
    float4 Weights : BONE_WEIGHT;
};

Skin SkinVertex(VS_IN input) {
    Skin output = (Skin)0;

    for (int i = 0; i < 4; i++) {
        matrix boneMat = BoneFramePose[input.Bones[i]];
        float weight = input.Weights[i];

        output.Pos4 += weight * mul(float4(input.Pos, 1), boneMat);
        output.Normal += weight * mul(input.Normal, (float3x3)boneMat);
    }
    return output;
}"""
    story.append(Preformatted(code4, code_style))
    story.append(Spacer(1, 10*mm))

    # 5. ゲームシステムアーキテクチャ
    story.append(PageBreak())
    story.append(Paragraph("5. Game System Architecture Design", heading2_style))
    story.append(Paragraph("<b>Location:</b> Src/07_Scene/, Src/08_Player/, Src/11_GameSystem/", body_style))
    story.append(Spacer(1, 3*mm))

    tech5_desc = """
    <b>Implementation:</b><br/>
    - Scene management system with lifecycle control<br/>
    - Object manager for game entity lifecycle<br/>
    - Game mechanics: Conical suction, experience system, Lerp interpolation<br/>
    <br/>
    <b>Technical Highlights:</b><br/>
    - Frame-rate independent physics using DeltaTime<br/>
    - Conical volume calculation for suction range<br/>
    - Height-based suction speed using easing curves<br/>
    - Internal division ratio for ground-plane projection
    """
    story.append(Paragraph(tech5_desc, body_style))
    story.append(Spacer(1, 3*mm))

    code5 = """// Suction displacement calculation
VECTOR3 CalcSuctionDisplacement(float moveTime,
                                const VECTOR3& animalPos) const {
    // Calculate speed multiplier based on height
    float heightDiff = m_coneTopPos - animalPos.y;
    float progress = 1.0f - (heightDiff / m_coneTopPos);
    float eased = progress * progress * progress; // Cubic easing

    float speedMultiplier = lerp(minSpeed, maxSpeed, eased);

    // Project to ground plane using internal division
    float ratio = (0 - animalPos.y) / (animalPos.y - m_coneTopPos);
    VECTOR3 groundPoint = VECTOR3(
        animalPos.x + ratio * (animalPos.x - transform.position.x),
        0,
        animalPos.z + ratio * (animalPos.z - transform.position.z)
    );

    VECTOR3 pullVector = transform.position - groundPoint;
    return pullVector / moveTime * speedMultiplier * DeltaTime();
}"""
    story.append(Preformatted(code5, code_style))
    story.append(Spacer(1, 10*mm))

    # まとめ
    story.append(PageBreak())
    story.append(Paragraph("Summary & Impact", heading1_style))
    story.append(Spacer(1, 5*mm))

    summary_text = """
    <b>Code Quality:</b><br/>
    - Used 'lizard' tool to measure cyclomatic complexity<br/>
    - Maintained low complexity scores through small, focused functions<br/>
    - Improved code readability, testability, and maintainability<br/>
    <br/>
    <b>Performance Optimization:</b><br/>
    - Spatial partitioning reduced collision checks by ~90%<br/>
    - State pre-allocation eliminated frame drops during transitions<br/>
    - Frame-rate independent physics ensures consistent gameplay<br/>
    <br/>
    <b>Software Engineering Principles:</b><br/>
    - Single Responsibility Principle in state design<br/>
    - DRY (Don't Repeat Yourself) with template programming<br/>
    - SOLID principles throughout architecture<br/>
    <br/>
    <b>Key Takeaways:</b><br/>
    These implementations demonstrate proficiency in:<br/>
    • Advanced C++ programming (templates, STL, modern C++11/14 features)<br/>
    • Algorithm design and optimization<br/>
    • Graphics programming (DirectX, HLSL)<br/>
    • Software architecture and design patterns<br/>
    • Performance-conscious development<br/>
    • Mathematical problem-solving for game mechanics
    """
    story.append(Paragraph(summary_text, body_style))

    # PDFをビルド
    doc.build(story)
    return pdf_filename

if __name__ == "__main__":
    try:
        filename = create_technical_resume_pdf()
        print(f"✓ PDF successfully created: {filename}")
    except Exception as e:
        print(f"✗ Error creating PDF: {e}")
        import traceback
        traceback.print_exc()
