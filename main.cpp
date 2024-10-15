#include <Novice.h>

const char kWindowTitle[] = "LC1C_20_フクダソウワ_ピカピカ風船";


/*-----------------
    構造体を作る
-----------------*/

// 座標
struct Coordinate2
{
	float x;
	float y;
};

// 位置
struct Pos
{
	// ワールド座標
	struct Coordinate2 world;

	// スクリーン座標
	struct Coordinate2 screen;
};

// 移動速度
struct Vel2
{
	float x;
	float y;
};

// 加速度
struct Acceleration2
{
	float x;
	float y;
};

// プレイヤーのフラグ
struct PlayerFlug
{
	// 空を飛んでいるかどうか（飛行フラグ）
	int isFlying;
};

// 図形の半径
struct Radius2
{
	float x;
	float y;
};


// プレイヤー
struct Player
{
	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 加速度
	struct Acceleration2 acceleration;

	// フラグ
	struct PlayerFlug flug;

	// 図形の半径
	struct Radius2 radius;
};

// 敵
struct Enemy
{
	// 出現しているかどうか（出現フラグ）
	int isArrival;

	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 図形の半径
	struct Radius2 radius;
};


/*---------------
    定数を作る
---------------*/

// 縦幅
const int kHeight = 800;

// 横幅
const int kWidth = 700;


/*----------------
    関数を作る
----------------*/

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="world">ワールド座標</param>
/// <returns>ワールド座標をスクリーン座標に変換して返却する</returns>
struct Coordinate2 CoordinateTransformation(struct Coordinate2 world);


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWidth, kHeight);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};


	/*----------------
	    変数を作る
	----------------*/

	/*   プレイヤー   */

	// 構造体
	struct Player player;

	// 位置
	player.pos.world = { kWidth / 2 , 0.0f };
	player.pos.screen = CoordinateTransformation(player.pos.world);

	// 移動速度
	player.vel = { 0.0f , 0.0f };

	// 加速度
	player.acceleration = { 0.0f , 0.0f };

	// フラグ
	player.flug.isFlying = false;

	// 図形の半径
	player.radius = { 10.0f , 10.0f };


	/*   画像   */

	// 白い図形
	int ghWhite = Novice::LoadTexture("./NoviceResources/white1x1.png");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		/*--------------------
		    プレイヤーの操作
		--------------------*/

		/* 飛行 */

		// スペースキーを押すと、飛行する（飛行フラグがtrueになる）
		if (keys[DIK_SPACE])
		{
			player.flug.isFlying = true;
		}
		else
		{
			// 押してないときは、飛行しない（飛行フラグがfalseになる）
			player.flug.isFlying = false;
		}

		// 飛行している（飛行フラグがtrueである）ときは、加速度が上昇する
		if (player.flug.isFlying)
		{
			if (player.acceleration.y < 1.0f)
			{
				player.acceleration.y += 0.1f;
			}
		}
		else
		{
			// 飛行していない（飛行フラグがfalseである）ときは、加速が減少する
			if (player.acceleration.y > -1.0f)
			{
				player.acceleration.y -= 0.1f;
			}
		}

		// 上下に動かす
		player.pos.world.y +=  3 * player.acceleration.y;


		/* 横移動 */

		// 横移動速度を初期化する
		player.vel.x = 0.0f;

		// 飛行中（飛行フラグがtrueであるとき）に、Aキーで左に移動できる
		if (keys[DIK_A])
		{
			if (player.flug.isFlying)
			{
				player.vel.x = -3.0f;
			}
		}

		// 飛行中（飛行フラグがtrueであるとき）に、Dキーで右に移動できる
		if (keys[DIK_D])
		{
			if (player.flug.isFlying)
			{
				player.vel.x = 3.0f;
			}
		}

		// 左右に動かす
		player.pos.world.x += player.vel.x;


		/*-------------------
		    座標変換を行う
		-------------------*/

		// プレイヤー
		player.pos.screen = CoordinateTransformation(player.pos.world);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		/*------------------------
		    図形や画像を描画する
		------------------------*/

		/*   プレイヤー   */

		Novice::DrawQuad
		(
			static_cast<int>(player.pos.screen.x - player.radius.x) , static_cast<int>(player.pos.screen.y - player.radius.y),
			static_cast<int>(player.pos.screen.x + player.radius.x), static_cast<int>(player.pos.screen.y - player.radius.y),
			static_cast<int>(player.pos.screen.x - player.radius.x), static_cast<int>(player.pos.screen.y + player.radius.y),
			static_cast<int>(player.pos.screen.x + player.radius.x), static_cast<int>(player.pos.screen.y + player.radius.y),
			0,0,1,1,ghWhite,0xFFFFFFFF
		);


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}


struct Coordinate2 CoordinateTransformation(struct Coordinate2 world)
{
	// 構造体
	struct Coordinate2 screen;

	screen.x = world.x;
	screen.y = -world.y + (kHeight - 100.0f);

	return screen;
}