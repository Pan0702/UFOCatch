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
    story.append(Paragraph("UFOCatch ゲームプロジェクト", title_style))
    story.append(Spacer(1, 10*mm))
    story.append(Paragraph("技術アピール資料", heading1_style))
    story.append(Spacer(1, 5*mm))
    story.append(Paragraph("DirectX & C++ ゲーム開発", body_style))
    story.append(PageBreak())

    # プロジェクト概要
    story.append(Paragraph("プロジェクト概要", heading1_style))
    story.append(Spacer(1, 5*mm))

    overview_text = """
    <b>ゲームコンセプト:</b> UFOを操作して人間に見つからないように動物を吸い込むステルスアクションゲーム<br/>
    <br/>
    <b>使用技術:</b><br/>
    - 開発言語: C++<br/>
    - グラフィックスAPI: DirectX 11<br/>
    - アーキテクチャ: コンポーネントベースのゲームエンジン<br/>
    - デザインパターン: State Pattern, Object Pool, Singleton<br/>
    <br/>
    <b>開発形態:</b> 個人開発<br/>
    <b>実装規模:</b> 約10,000行以上の独自実装コード
    """
    story.append(Paragraph(overview_text, body_style))
    story.append(Spacer(1, 10*mm))

    # 技術アピールポイント
    story.append(PageBreak())
    story.append(Paragraph("4つの主要技術実装", heading1_style))
    story.append(Spacer(1, 10*mm))

    # 1. 空間分割アルゴリズム
    story.append(Paragraph("1. 空間分割アルゴリズム（線形4分木 + モートン順序）", heading2_style))
    story.append(Paragraph("<b>実装箇所:</b> Src/09_Enemy/Liner4Tree.h", body_style))
    story.append(Spacer(1, 3*mm))

    tech1_desc = """
    <b>実装内容:</b><br/>
    - 4分木による効率的な衝突判定の最適化<br/>
    - モートン曲線（Z曲線）を使ったビット演算による空間インデックス<br/>
    - 衝突判定の計算量を O(n²) から O(n log n) に削減<br/>
    <br/>
    <b>技術的なポイント:</b><br/>
    - ビット分離アルゴリズムによるモートン符号化<br/>
    - 階層的なセル探索による空間クエリ<br/>
    - テンプレートによる汎用的なゲームオブジェクトコンテナ
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
    story.append(Paragraph("2. 幾何学的衝突判定システム", heading2_style))
    story.append(Paragraph("<b>実装箇所:</b> Src/11_GameSystem/VisionSystem.h/cpp", body_style))
    story.append(Spacer(1, 3*mm))

    tech2_desc = """
    <b>実装内容:</b><br/>
    - AI視界システムのための扇形と円の高度な衝突判定<br/>
    - 2次方程式の判別式を使った線分と円の交差判定<br/>
    - 敵AIの発見メカニクスへの応用<br/>
    <br/>
    <b>技術的なポイント:</b><br/>
    - 内積とベクトル正規化を用いた数学的アプローチ<br/>
    - 早期リジェクションテストによる計算の最小化<br/>
    - 扇形の境界や円弧の交差などのエッジケースに対応
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
    story.append(Paragraph("3. デザインパターンの実装（State Pattern）", heading2_style))
    story.append(Paragraph("<b>実装箇所:</b> Src/12_Tutorial/TutorialState.h, Src/09_Enemy/Human/State/", body_style))
    story.append(Spacer(1, 3*mm))

    tech3_desc = """
    <b>実装内容:</b><br/>
    - 複雑な振る舞い管理のためのステートマシン<br/>
    - チュートリアル進行: Move → Suction → Expand → Discovery → Play<br/>
    - 敵AIの状態遷移: Idle → Walk → FindPlayer<br/>
    <br/>
    <b>技術的なポイント:</b><br/>
    - 関心の分離: ActorクラスとStateクラスの分離<br/>
    - std::unordered_map による O(1) の状態検索<br/>
    - 事前割り当てによる実行時のメモリ確保の排除<br/>
    - 状態遷移時のフレームレート低下を防止
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

    # 4. ゲームシステムアーキテクチャ
    story.append(PageBreak())
    story.append(Paragraph("4. ゲームシステムアーキテクチャ設計", heading2_style))
    story.append(Paragraph("<b>実装箇所:</b> Src/07_Scene/, Src/08_Player/, Src/11_GameSystem/", body_style))
    story.append(Spacer(1, 3*mm))

    tech4_desc = """
    <b>実装内容:</b><br/>
    - ライフサイクル制御を持つシーン管理システム<br/>
    - ゲームエンティティのライフサイクルを管理するオブジェクトマネージャ<br/>
    - ゲームメカニクス: 円錐形吸引、経験値システム、Lerp補間<br/>
    <br/>
    <b>技術的なポイント:</b><br/>
    - DeltaTimeを使用したフレームレート非依存の物理演算<br/>
    - 吸引範囲の円錐体積計算<br/>
    - イージングカーブを使った高さベースの吸引速度<br/>
    - 内分比による地面平面への投影計算
    """
    story.append(Paragraph(tech4_desc, body_style))
    story.append(Spacer(1, 3*mm))

    code4 = """// Suction displacement calculation
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
    story.append(Preformatted(code4, code_style))
    story.append(Spacer(1, 10*mm))

    # まとめ
    story.append(PageBreak())
    story.append(Paragraph("まとめと成果", heading1_style))
    story.append(Spacer(1, 5*mm))

    summary_text = """
    <b>コード品質:</b><br/>
    - 「lizard」ツールを使用して循環的複雑度を測定<br/>
    - 小さく焦点を絞った関数で低い複雑度スコアを維持<br/>
    - コードの可読性、テスト容易性、保守性を向上<br/>
    <br/>
    <b>パフォーマンス最適化:</b><br/>
    - 空間分割により衝突判定を約90%削減<br/>
    - 状態の事前割り当てにより遷移時のフレーム落ちを排除<br/>
    - フレームレート非依存の物理演算で一貫したゲームプレイを実現<br/>
    <br/>
    <b>ソフトウェアエンジニアリング原則:</b><br/>
    - ステート設計における単一責任の原則<br/>
    - テンプレートプログラミングによるDRY原則<br/>
    - アーキテクチャ全体でSOLID原則を適用<br/>
    <br/>
    <b>習得技術:</b><br/>
    これらの実装により以下の技術を実証:<br/>
    • 高度なC++プログラミング（テンプレート、STL、モダンC++11/14機能）<br/>
    • アルゴリズム設計と最適化<br/>
    • グラフィックスプログラミング（DirectX）<br/>
    • ソフトウェアアーキテクチャとデザインパターン<br/>
    • パフォーマンスを意識した開発<br/>
    • ゲームメカニクスのための数学的問題解決
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
