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

// 復活
struct Respawn
{
	// 復活しているかどうか（復活フラグ）
	int isRespawn;

	// 復活処理
	int timer;
};

// プレイヤーのフラグ
struct PlayerFlug
{
	// 空を飛んでいるかどうか（飛行フラグ）
	int isFlying;
};


// プレイヤー
struct Player
{
	// 復活
	struct Respawn respawn;

	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 加速度
	struct Acceleration2 acceleration;

	// フラグ
	struct PlayerFlug flug;

	// 図形の半径
	float radius;
};

// 敵
struct Enemy
{
	// 出現しているかどうか（出現フラグ）
	int isArrival;

	// 種類
	int type;

	// 復活
	struct Respawn respawn;

	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 図形の半径
	float radius;

	// 透明度
	int transparency;
};

// アイテム
struct Item
{
	// 出現しているかどうか（出現フラグ）
	int isArrival;

	// 復活
	struct Respawn respawn;

	// 位置
	struct Pos pos;

	// 移動速度
	struct Vel2 vel;

	// 図形の半径
	float radius;
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


// 復活処理の初期値
const int initialValueRespawnTimer = 120;


/*----------------
    関数を作る
----------------*/

/// <summary>
/// 座標変換を行う
/// </summary>
/// <param name="world">ワールド座標</param>
/// <returns>ワールド座標をスクリーン座標に変換して返却する</returns>
struct Coordinate2 CoordinateTransformation(struct Coordinate2 world);

/// <summary>
/// 復活処理を行う
/// </summary>
/// <param name="respawn">復活</param>
void RespawnProcess(struct Respawn* respawn);

/// <summary>
/// 敵の数値を入れる
/// </summary>
/// <param name="enemy">敵</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="radius">図形の半径</param>
void MakeEnemy(struct Enemy* enemy, float posX, float posY, float velX, float velY, float radius , int type);

/// <summary>
/// アイテムの数値を入れる
/// </summary>
/// <param name="item">アイテム</param>
/// <param name="posX">X軸の位置</param>
/// <param name="posY">Y軸の位置</param>
/// <param name="velX">X軸の移動速度</param>
/// <param name="velY">Y軸の移動速度</param>
/// <param name="radius">図形の半径</param>
void MakeItem(struct Item* item, float posX, float posY, float velX, float velY, float radius);


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


	// 逃げているかどうか（逃げるフラグ）
	int isRunAway = false;


	/*   プレイヤー   */

	// 構造体
	struct Player player;

	// 復活
	player.respawn.isRespawn = false;
	player.respawn.timer = 120;

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
	player.radius = 10.0f;


	/*   敵   */

	// 構造体
	struct Enemy enemy[kEnemyNum];

	// 敵の種類
	enum ENEMY_TYPE
	{
		ENEMY_TYPE_STONE,
		ENEMY_TYPE_DENGER
	};

	for (int i = 0; i < kEnemyNum; i++)
	{
		// 出現しているかどうか（出現フラグ）
		enemy[i].isArrival = false;

		// 復活
		enemy[i].respawn.isRespawn = true;
		enemy[i].respawn.timer = 120;

		// 種類
		enemy[i].type = -1;

		// 位置
		enemy[i].pos.world = { 0.0f , 0.0f };
		enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);

		// 移動速度
		enemy[i].vel = { 0.0f , 0.0f };

		// 図形の半径
		enemy[i].radius = 0.0f;

		// 不透明度
		enemy[i].transparency = 0;
	}


	/*   アイテム   */

	struct Item item;

	// 出現しているかどうか
	item.isArrival = false;

	// 復活
	item.respawn.isRespawn = true;
	item.respawn.timer = 120;

	// 位置
	item.pos = { 0.0f , 0.0f };

	// 移動速度
	item.vel = { 0.0f , 0.0f };

	// 図形の半径
	item.radius = 0.0f;




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

					player.respawn.isRespawn = true;

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
					MakeItem(&item , 350.0f , 650.0f , 0.0f , 0.0f , 20.0f);

					MakeEnemy(enemy , 350.0f , 500.0f , 0.0f , 0.0f , 50.0f , ENEMY_TYPE_STONE);

					MakeEnemy(enemy, 100.0f, 300.0f, 0.0f, 0.0f, 50.0f, ENEMY_TYPE_STONE);
				}

				break;

			case STAGE_TYPE_2:

				break;
			}

			// ゲームフレームを動かす
			gameFrame++;


			/*---------------
			    復活処理
			---------------*/

			// プレイヤー
			RespawnProcess(&player.respawn);

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				RespawnProcess(&enemy[i].respawn);
			}

			// アイテム
			RespawnProcess(&item.respawn);


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

				// 風船が膨らむ
				if (player.radius < 50.0f)
				{
					player.radius += 0.5f;
				}
			} 
			else
			{
				// 飛行していない（飛行フラグがfalseである）ときは、加速が減少する
				if (player.acceleration.y > -1.0f)
				{
					player.acceleration.y -= 0.1f;
				}

				// 風船が縮む
				if (player.radius > 10.0f)
				{
					player.radius--;
				}
				else
				{
					// 通常より小さくなってはいけない
					player.radius = 10.0f;
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
					if (player.pos.world.x > 0.0f)
					{
						player.vel.x = -3.0f;
					}
				}
			}

			// 飛行中（飛行フラグがtrueであるとき）に、Dキーで右に移動できる
			if (keys[DIK_D])
			{
				if (player.flug.isFlying)
				{
					if (player.pos.world.x < static_cast<float>(kWidth))
					{
						player.vel.x = 3.0f;
					}
				}
			}

			// 左右に動かす
			player.pos.world.x += player.vel.x;

			// 画面外に出ないようにする
			if (player.pos.world.x - player.radius < 0.0f)
			{
				player.pos.world.x = player.radius;
			}

			if (player.pos.world.x + player.radius > static_cast<float>(kWidth))
			{
				player.pos.world.x = static_cast<float>(kWidth) - player.radius;
			}

			if (player.pos.world.y - player.radius < 0.0f)
			{
				player.pos.world.y = player.radius;
			}

			if (player.pos.world.y + player.radius > static_cast<float>(kHeight) - 100.0f)
			{
				player.pos.world.y = static_cast<float>(kHeight) - 100.0f - player.radius;
			}


			/*----------------
				当たり判定
			----------------*/

			// プレイヤーの光 と 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				if (enemy[i].isArrival)
				{
					if (powf(player.radius + enemy[i].radius + 100.0f, 2) >
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
					if (powf(player.radius + enemy[i].radius, 2) >=
						powf(player.pos.world.x - enemy[i].pos.world.x, 2) + powf(player.pos.world.y - enemy[i].pos.world.y, 2))
					{
						// プレイヤーがやられる（復活フラグがfalseになる）
						player.respawn.isRespawn = false;
					}
				}
			}


			// プレイヤー と アイテム
			if (player.respawn.isRespawn)
			{
				if (item.isArrival)
				{
					if (powf(player.radius + item.radius, 2) >=
						powf(player.pos.world.x - item.pos.world.x, 2) + powf(player.pos.world.y - item.pos.world.y, 2))
					{
						// アイテムが消える（出現、復活）
						item.isArrival = false;
						item.respawn.isRespawn = false;

						// プレイヤーが逃げ始める（逃げるフラグがtrueになる）
						isRunAway = true;
					}
				}
			}


			/*-------------------
				座標変換を行う
			-------------------*/

			// ふっかつしているプレイヤー
			player.pos.screen = CoordinateTransformation(player.pos.world);

			// 敵
			for (int i = 0; i < kEnemyNum; i++)
			{
				enemy[i].pos.screen = CoordinateTransformation(enemy[i].pos.world);
			}

			// アイテム
			item.pos.screen = CoordinateTransformation(item.pos.world);

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

		if (player.respawn.isRespawn)
		{
			Novice::DrawEllipse
			(
				static_cast<int>(player.pos.screen.x), static_cast<int>(player.pos.screen.y),
				static_cast<int>(player.radius), static_cast<int>(player.radius),
				0.0f, 0xFFFFFFFF, kFillModeSolid
			);
		}

		
		/*   敵   */

		for (int i = 0; i < kEnemyNum; i++)
		{
			if (enemy[i].isArrival)
			{
				Novice::DrawEllipse
				(
					static_cast<int>(enemy[i].pos.screen.x), static_cast<int>(enemy[i].pos.screen.y),
					static_cast<int>(enemy[i].radius) , static_cast<int>(enemy[i].radius) , 
					0.0f , 0xFFFFFF00 + enemy[i].transparency, kFillModeSolid
				);
			}
		}


		/*   アイテム   */

		if (item.isArrival)
		{
			Novice::DrawEllipse
			(
				static_cast<int>(item.pos.screen.x) , static_cast<int>(item.pos.screen.y),
				static_cast<int>(item.radius) , static_cast<int>(item.radius),
				0.0f , 0xFFFF00FF , kFillModeSolid
			);
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
	screen.y = -world.y + (static_cast<float>(kHeight) - 100.0f);

	return screen;
}

void RespawnProcess(struct Respawn* respawn)
{
	// ヌルを探す
	if (respawn == nullptr)
	{
		return;
	}

	// やられている（復活フラグがfalseである）ときに行う
	if (respawn->isRespawn == false)
	{
		respawn->timer--;

		if (respawn->timer <= 0)
		{
			// 復活する（復活フラグがtrueになる）
			respawn->isRespawn = true;

			// 復活処理を初期化する
			respawn->timer = initialValueRespawnTimer;
		}
	}
}


void MakeEnemy(struct Enemy* enemy, float posX, float posY, float velX, float velY, float radius , int type)
{
	// nullを探す
	if (enemy == nullptr)
	{
		return;
	}

	for (int i = 0; i < kEnemyNum; i++)
	{
		if (enemy[i].isArrival == false)
		{
			// 敵を出現させる（出現フラグがtrueになる）
			enemy[i].isArrival = true;

			// 敵の種類
			enemy[i].type = type;

			// 配置する
			enemy[i].pos.world = { posX , posY };

			// 移動速度を指定する
			enemy[i].vel = { velX , velY };

			// 図形の半径
			enemy[i].radius = radius;

			break;
		}
	}
}


void MakeItem(struct Item* item, float posX, float posY, float velX, float velY, float radius)
{
	// nullを探す
	if (item == nullptr)
	{
		return;
	}

	// アイテムを出現させる（出現フラグをtrueにする）
	item->isArrival = true;

	// 位置
	item->pos.world = { posX , posY };

	// 移動速度
	item->vel = { velX , velY };

	// 図形の半径
	item->radius = radius;
}