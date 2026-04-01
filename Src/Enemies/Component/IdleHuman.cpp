#include "IdleHuman.h"
#include "../System/EnemyManager.h"
#include "../../Utils/Lerp.h"

#include "../Human/Human.h"

////////////////////
// コンストラクタ
// @param e このコンポーネントの所有者（人間キャラクター）
////////////////////
CIdleHuman::CIdleHuman(CHuman* e)
{
    m_pOwner = e;
}


////////////////////
// 待機状態に入る時の初期化処理
// ランダムで「通常の待機アニメーション」か「周りを見渡すアニメーション」を選択する
////////////////////
void CIdleHuman::Enter()
{
    // 首の角度と経過時間を初期化 //
    currentAngle = 0;
    animationTime = 0;
    m_isFinish = false;

    // 0か1のランダムな値で待機アニメーションの種類を決定 //
    m_idleState = static_cast<int>(std::round(Randomf(0, 1)));
    if (m_idleState)
    {
        // 通常の待機アニメーションを再生 //
        m_pOwner->GetAnimator()->MergePlay(A_IDEL);
    }
    else
    {
        // 周りを見渡すアニメーションを再生 //
        m_pOwner->GetAnimator()->MergePlay(A_SEACH);
    }
}

////////////////////
// 毎フレーム呼ばれる更新処理
// 基底クラスの更新処理を呼び出す
////////////////////
void CIdleHuman::Update()
{
    switch (m_idleState)
    {
    case ANIMATION: IdleAnim();
        break;
    case SEARCH: IdleSearch();
        break;
    default: assert("error:idleHumanState");
        break;
    }
}

////////////////////
// 待機状態を終了する時の処理
// 基底クラスの終了処理を呼び出す
////////////////////
void CIdleHuman::Exit()
{
    CComponentBase::Exit();
}

////////////////////
// 通常の待機アニメーション処理
// （現在は未実装）
////////////////////
void CIdleHuman::IdleAnim()
{
    if (AnimationFinish())
    {
        m_isFinish = true;
    }
}

////////////////////
// 周りを見渡すアニメーションの終了判定
// アニメーションが終了したら首の角度をリセットし、待機状態を終了する
////////////////////
void CIdleHuman::IdleSearch()
{
    // アニメーションが終了したか確認 //
    if (m_pOwner->GetAnimator()->Finished())
    {
        // 首の角度を正面（0度）にリセット //
        m_pOwner->SetAngle(0);

        // 待機状態を終了 //
        m_isFinish = true;
    }
}

////////////////////
// 人間キャラクター用の「周りを見渡す」アニメーションを実行する
// 首を左右に回転させながら、アニメーションフレームに応じた角度を計算する
// アニメーション全体は100フレームで構成され、以下の順序で動作する：
// 1. 待機：1-6フレーム
// 2. 左を見る：7-21フレーム (回転) -> 保持：22-33フレーム -> 正面に戻る：34-47フレーム
// 3. 右を見る：48-62フレーム (回転) -> 保持：63-74フレーム -> 正面に戻る：75-88フレーム
// 4. 終了：89-100フレーム
////////////////////
void CIdleHuman::LookAroundAnim()
{
    float currentFrame = GetCurrentFrame();

    // アニメーション全体の総フレーム数 //
    constexpr float TOTAL_FRAMES = 100.0f;
    if (currentFrame >= TOTAL_FRAMES)
    {
        currentAngle = 0.0f;
        return;
    }

    // 首の回転アニメーションが終了するフレーム番号（それ以降は待機状態） //
    constexpr float NECK_ANIMATION_END = 86.0f;
    if (currentFrame > NECK_ANIMATION_END)
    {
        return;
    }

    // 首の回転が開始されるまでの待機フレーム数 //
    constexpr float WAIT_START_FRAME = 6.0f;
    if (currentFrame <= WAIT_START_FRAME)
    {
        currentAngle = 0.0f;
    }
    else
    {
        // 待機フレームを引いた実際の動作フレーム数を計算 //
        float cycleFrame = currentFrame - WAIT_START_FRAME;

        // 左右を見回す1サイクルの合計フレーム数（左→正面→右→正面） //
        constexpr float CYCLE_DURATION = 81.0f;

        if (cycleFrame <= CYCLE_DURATION)
        {
            // 半サイクル（片側を見る動作全体）のフレーム数 //
            constexpr float HALF_CYCLE_DURATION = 41.0f;
            // 現在が左右どちらのサイクルかを判定（0:左、1:右） //
            int halfCycle = static_cast<int>((cycleFrame - 1.0f) / HALF_CYCLE_DURATION);
            // 各半サイクル内でのローカルフレーム番号（0-40） //
            float localFrame = fmodf(cycleFrame - 1.0f, HALF_CYCLE_DURATION);
            // 首を左右に回転させる角度（度数法） //
            constexpr float ANGLE = 50.0f;
            // 左を見る場合は+50度、右を見る場合は-50度 //
            float targetAngle = (halfCycle == 0) ? ANGLE : -ANGLE;
            // 首を回転させる動作にかかるフレーム数（補間期間） //
            constexpr float CHANGE_DURATION = 15.0f;
            // 首を回転させた状態を保持する終了フレーム //
            constexpr float HOLD_END_FRAME = 27.0f;
            // 首を元の位置に戻し終わるフレーム //
            constexpr float RETURN_END_FRAME = 41.0f;

            // 1：正面から左（または右）へ回転する（0-14フレーム） //
            if (localFrame < CHANGE_DURATION)
            {
                // 首の回転補間計算用の除数 (CHANGE_DURATION - 1) //
                constexpr float CHANGE_DIVISOR = 14.0f;
                float t = localFrame / CHANGE_DIVISOR; // 補間係数（0.0 → 1.0） //
                currentAngle = Lerp(0.0f, targetAngle, t);
            }
            // 2：回転した状態を保持する（15-26フレーム） //
            else if (localFrame < HOLD_END_FRAME)
            {
                currentAngle = targetAngle;
            }
            // 3：左（または右）から正面へ戻る（27-40フレーム） //
            else if (localFrame < RETURN_END_FRAME)
            {
                // 首を元の位置に戻し始めるフレーム //
                constexpr float RETURN_START_FRAME = 27.0f;

                // 首の復帰補間計算用の除数 (RETURN_END_FRAME - RETURN_START_FRAME - 1) //
                constexpr float RETURN_DIVISOR = 13.0f;

                float t = (localFrame - RETURN_START_FRAME) / RETURN_DIVISOR; // 補間係数（0.0 → 1.0） //
                currentAngle = Lerp(targetAngle, 0.0f, t);
            }
        }
    }

    // 計算した角度をラジアンに変換してオーナーに適用 //
    m_pOwner->SetAngle(currentAngle * DegToRad);
}


////////////////////
// 経過時間から現在のアニメーションフレーム番号を計算する
// @return 現在のフレーム番号（浮動小数点数）
////////////////////
float CIdleHuman::GetCurrentFrame() const
{
    // アニメーションのフレームレート（1秒あたりのフレーム数） //
    constexpr float ANIMATION_FPS = 30.0f;

    // 経過時間（秒）× FPS = フレーム番号 //
    return animationTime * ANIMATION_FPS;
}

////////////////////
// アニメーションが終了したかを判定する
// （現在は未実装）
// @return 終了判定の結果
////////////////////
bool CIdleHuman::AnimationFinish() const
{
    if (m_pOwner->GetAnimator()->CurrentFrame() >= 175.0f)
    {
        return true;
    }
    return false;
}
