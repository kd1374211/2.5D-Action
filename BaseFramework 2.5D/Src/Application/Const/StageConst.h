#pragma once

//階段の最大（最小）座標
const float STAIRPOSY_MIN = -3.28f;
const float STAIRPOSY_MAX = 8.24f;

//再生成時の位置変化
const float STAIRRESPAWNY = 11.52f;

//壁・柱の最大（最小）座標
const float PILLARWALLPOSY_MIN = -10.0f;
const float PILLARWALLPOSY_MAX = 0.0f;

//再生成時の位置変化
const float PILLARWALLRESPAWNY = 10.0f;

//再生成の方向
enum class RespawnDir
{
	Down = -1,
	Up = 1
};

//実際に動ける範囲（中央からの距離）
const float LINEPLAYAREA_MIN = 2.0f;
const float LINEPLAYAREA_MAX = 8.0f;

//地形の基礎スクロール速度
const float TERRAINBASEMOVEY = -0.016f;
const float TERRAINBASEROTATY = 1.0f;