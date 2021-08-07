# Near マップ形式 v1

## 導入

Near マップ形式はゲームのような何か Near 専用プレーンテキストマップ記述言語です。

例:

```
Near Map v1

block { position     0 -8 0 size 640 16 640 faces 0 0 0 0 1 0 }
block { position 10000 -8 0 size 640 16 640 faces 0 0 0 0 2 0 }

# Gate
block { position -80  64 256 size  32 128 32 faces 4 4 4 4 4 0 }
block { position  80  64 256 size  32 128 32 faces 4 4 4 4 4 0 }
block { position   0 144 256 size 192  32 32 faces 4 4 4 4 4 4 }
portal { name portal_a partner portal_b position 0 64 256 size 128 128 }

# Gate
block { position  9920  64 256 size  32 128 32 faces 4 4 4 4 4 0 }
block { position 10080  64 256 size  32 128 32 faces 4 4 4 4 4 0 }
block { position 10000 144 256 size 192  32 32 faces 4 4 4 4 4 4 }
portal { name portal_b partner portal_a position 10000 64 256 size 128 128 }

```

## 基本構成

### 行とトークン

Near マップ形式では、データは行またはトークンで表されます。行は改行文字 (\nまたは\r\n) で区切った、トークンは空白文字 (\f \n \r \t \v または半角スペース) で区切ったひとまとめです。行とトークンは空ではいけません。

サンプル:

```
Near Map v1

block {
  position   0 -8   0
  size     640 16 640
}
```

全部行で分けた例:

```
Near Map v1
block {
  position   0 -8   0
  size     640 16 640
}
```

全部トークンで分けた例:

```
Near
Map
v1
block
{
position
0
-8
0
size
640
16
640
}
```

### プロパティ

このドキュメントでは、種類を表すトークンと、種類ごとに固定の形式・個数ののセットをプロパティと呼びます。

例:

```
name 三ツ矢サイダーゼロストロング
position 0 128 0
```

### オブジェクトとエンティティ

オブジェクトは複数のトークンで構成されたデータのひとまとめです。

構文:

1. オブジェクトの種類 (任意のトークン)
1. トークン "{"
1. 中身 (任意の形式)
1. トークン "}"

例:

```
type { foo bar 123 456 }
 ^   ^  ^   ^   ^   ^  ^
 1   2  3   3   3   3  4
```

一般的なオブジェクトは中身がプロパティ列になります:

```
objectType {
  property value value value
  property value value
}
```

以下のプロパティを必ず持つオブジェクトはエンティティと呼びます:

- `string name`: エンティティ名
- `float3 position`: 位置
- `float3 rotation`: 回転 (yaw/pitch/roll)

### ファイル構造

Near マップファイルは、最初に行"Near Map v1"、そのあとはファイル終端までブロックが並びます。

### データ形式

- `int`: 32ビット符号付整数
- `float`: 単精度浮動小数点数
- `xxx[n]`: 配列とかベクトルとかほらそういう系のやつ
- `string`: UTF-8文字列 (行とトークンの仕様上、空白は入れられません)

## オブジェクトリファレンス

### `block`

仮想的な直方体の面を使って、マップ上に表示されるポリゴンを追加します。

プロパティ:

- `float3 position`: 中心位置
- `float3 size`: 大きさ
- `float3 rotation`: 回転 (現在不使用、yaw/pitch/roll)

## エンティティリファレンス

### `portal`

マップにポータルを追加します。

プロパティ:

- `string partner`: このポータルがつながるもう一つのポータルのエンティティ名
