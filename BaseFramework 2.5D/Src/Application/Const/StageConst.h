#pragma once

//階段の最大（最小）座標
const float STAIRPOSY_MIN = -30.0f;
const float STAIRPOSY_MAX = 30.0f;

//再生成時の位置変化
const float STAIRRESPAWNY = 60.0f;

//壁・柱の最大（最小）座標
const float PILLARWALLPOSY_MIN = -96.0f;
const float PILLARWALLPOSY_MAX = 96.0f;

//再生成時の位置変化
const float PILLARWALLRESPAWNY = 192.0f;

//再生成の方向
enum class RespawnDir
{
	Down = -1,
	Up = 1
};

//実際に動ける範囲（中央からの距離）
const float LINEPLAYAREA_MIN = 8.0f;
const float LINEPLAYAREA_MAX = 36.0f;

//地形の基礎スクロール速度
const float TERRAINBASEMOVEY = -0.075f;
const float TERRAINBASEROTATY = 0.9f;