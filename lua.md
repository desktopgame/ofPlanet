# PLANET lua api document

[LUA Manual](https://www.lua.org/manual/5.3/)

## コールバック関数

### getCallbackMode -> mode
スクリプトで記述するアルゴリズムに応じてコールバックの種類を決めます。  
* default
    * 全てのコールバックを呼び出します。
* ignore
    * onPostGenerateだけを呼び出します。

### onFixHeight(y) -> y
y: -1 ~ 1 で表される2Dノイズ
パーリンノイズ2Dで生成されたy座標を必要ならフィルタできます。

### onGenerateTerrain(x, y, z) -> void
x, y, z: 生成先の座標
地表にブロックを配置します。

### onGenerateWater(x, y, z) -> void
x, y, z: 生成先の座標
水を生成します。

### onGenerateStructures() -> void
構造物を生成します。

### onGenerateCave(x, y, z, noise) -> void
x, y, z: 生成先の座標
noise: -1 ~ 1 で表される3Dノイズ
洞窟を生成します。

### onPostGenerate -> void
一番最後に呼び出されるコールバックです。

## 関数

### setblock(x, y, z, name)
指定座標のブロックを設定します  
nameが空文字列のとき、指定位置のブロックを削除します

### getblock(x, y, z) -> name
指定座標のブロックを返します

### getxsize() -> xsize
ワールドのX方向のサイズを返します

### getysize() -> ysize
ワールドのY方向のサイズを返します

### getzsize() -> zsize
ワールドのZ方向のサイズを返します

## その他

### ブロックを指定する方法
`blocks.json`に定義されているブロックの `reference` 属性と同じ名前を使用してください。