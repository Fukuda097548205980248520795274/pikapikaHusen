#include <Novice.h>
#include <math.h>

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

	// 透明度
	int transparency;
};


/*---------------
    定数を作る
---------------*/

// 縦幅
const int kHeight = 800;

// 横幅
const int kWidth = 700;


// 敵の数
const int kEnemyNum = 256;


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

	/*   ゲーム   */

	// ゲームフレーム
	int gameFrame = 0;

	// ゲームが動いているか（ゲームフラグ）
	int isGameOperation = false;

	// 画面の種類
	enum SCREEN_TYPE
	{
		SCREEN_TYPE_START,
		SCREEN_TYPE_GAME,
	};

	// 現在の画面
	int screenNo = SCREEN_TYPE_START;

	// ステージの種類
	enum STAGE_TYPE
	{
		STAGE_TYPE_1,
		STAGE_TYPE_2
	};

	// 現在のステージ
	int stageNo = -1;


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


	/*   敵   */

	// 構造体
	struct Enemy enemy[kEnemyNum];

	for (int i = 0; i < kEnemyNum; i++)
	{
		// 出現しているかどうか（出現フラグ）
		enemy[i].isArrival = false;

		// 位置
		enemy[i].pos.world = { 0.0f , 0.0f };
		enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

		// 移動速度
		enemy[i].vel = { 0.0f , 0.0f };

		// 図形の半径
		enemy[i].radius = { 0.0f , 0.0f };

		// 不透明度
		enemy[i].transparency = 0;
	}


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

		switch (screenNo)
		{
		case SCREEN_TYPE_START:

			///
			/// ↓ スタート画面ここから
			/// 

			// ゲームが動いている（ゲームフラグがtrueである）ときに、ゲームフレームも動く
			if (isGameOperation)
			{
				gameFrame++;
			}

			// スペースキーでゲームを開始する
			if (keys[DIK_SPACE])
			{
				if (isGameOperation == false)
				{
					isGameOperation = true;
				}
			}

			if (isGameOperation)
			{
				if (gameFrame >= 70)
				{
					screenNo = SCREEN_TYPE_GAME;

					stageNo = STAGE_TYPE_1;

					gameFrame = 0;
				}
			}

			///
			/// ↑ スタート画面ここまで
			/// 

			break;

		case SCREEN_TYPE_GAME:

			///
			/// ↓ ゲーム画面ここから
			/// 

			/*------------------
				ステージの設定
			------------------*/

			switch (stageNo)
			{
			case STAGE_TYPE_1:

				// 0フレームで、障害物を出現させる
				if (gameFrame == 0)
				{
					for (int i = 0; i < kEnemyNum; i++)
					{
						if (enemy[i].isArrival == false)
						{
							// 敵を出現させる（出現フラグがtrueになる）
							enemy[i].isArrival = true;

							// 敵を配置する
							enemy[i].pos.world = { 350.0f , 500.0f };

							// 図形の半径
							enemy[i].radius = { 100.0f , 100.0f };

							break;
						}
					}
				}

				break;

			case STAGE_TYPE_2:

				break;
			}

			// ゲームフレームを動かす
			gameFrame++;


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
			player.pos.world.y += 3 * player.acceleration.y;


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


			/*----------------
				当たり判定
			----------------*/

			// プレイヤーの光 と 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				if (enemy[i].isArrival)
				{
					if (powf(player.radius.x + enemy[i].radius.x + 150.0f, 2) >
						powf(enemy[i].pos.world.x - player.pos.world.x, 2) + powf(enemy[i].pos.world.y - player.pos.world.y, 2))
					{
						// プレイヤーが飛行している（飛行フラグがtrueになる）と、光る
						if (player.flug.isFlying)
						{
							// 敵が光る
							if (enemy[i].transparency < 255)
							{
								enemy[i].transparency++;
							}
						} 
						else
						{
							// 敵がみえなくなっていく
							if (enemy[i].transparency > 0)
							{
								enemy[i].transparency -= 3;
							}
						}
					}
					else
					{
						// 光が触れていないとき、敵がみえなくなっていく
						if (enemy[i].transparency > 0)
						{
							enemy[i].transparency--;
						}
					}

					// 敵の光に下限をつける
					if (enemy[i].transparency < 0)
					{
						enemy[i].transparency = 0;
					}
				}
			}


			// プレイヤー と 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				if (enemy[i].isArrival)
				{
					if (player.pos.world.x + player.radius.x > enemy[i].pos.world.x - enemy[i].radius.x &&
						player.pos.world.x - player.radius.x < enemy[i].pos.world.x + enemy[i].radius.x)
					{
						if (player.pos.world.y + player.radius.y > enemy[i].pos.world.y - enemy[i].radius.y &&
							player.pos.world.y - player.radius.y < enemy[i].pos.world.y + enemy[i].radius.y)
						{
							enemy[i].isArrival = false;
						}
					}
				}
			}


			/*-------------------
				座標変換を行う
			-------------------*/

			// プレイヤー
			player.pos.screen = CoordinateTransformation(player.pos.world);

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);
			}

			///
			/// ↑ ゲーム画面ここまで
			/// 

			break;
		}

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

		
		/*   敵   */

		for (int i = 0; i < kEnemyNum; i++)
		{
			if (enemy[i].isArrival)
			{
				Novice::DrawQuad
				(
					static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
					static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y - enemy[i].radius.y),
					static_cast<int>(enemy[i].pos.screen.x - enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
					static_cast<int>(enemy[i].pos.screen.x + enemy[i].radius.x), static_cast<int>(enemy[i].pos.screen.y + enemy[i].radius.y),
					0, 0, 1, 1, ghWhite, 0xFFFFFF00 + enemy[i].transparency
				);
			}
		}


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