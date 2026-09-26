# CLAUDE.md

このファイルは、このリポジトリで作業する Claude Code 向けのガイドです。

## プロジェクト概要

- **petitcon26**: Unreal Engine 5.8 で作っているドッジボールゲーム（プチコン向けの小規模プロジェクト）。
- プレイヤーは体育館のコートでボールを投げ、敵に当てるとラグドール化して吹っ飛ぶ。
- UE の Blank テンプレート（`TP_Blank`）から作ったもの。C++ で基本クラスを作り、Blueprint で継承して値やアセットを設定する構成。

## ビルドと実行

- エンジン: UE 5.8（`petitcon26.uproject` の `EngineAssociation`）。開発者の環境は Windows（`D:/Program Files/Epic Games/UE_5.8`）。
- ソリューション: `petitcon26.slnx` / `Automation_petitcon26.slnx`（エンジンが生成したファイル）。
- C++ モジュール: `petitcon26`（Runtime）。ターゲットは `petitcon26`（Game）と `petitcon26Editor`（Editor）。
- このクラウド環境には UE がないので、**ビルド、テスト、エディタ起動はできない**。C++ を変更したときは、UE の API と既存コードを見比べて慎重に確認すること。
- 自動テストや Lint はない。

## ディレクトリ構成

```
Source/petitcon26/        C++ ゲームモジュール
  DodgeBallPlayer.*       プレイヤー (ACharacter)
  DodgeBall.*             投げるボール (AActor)
  Enemy.*                 敵 (APawn)
  DefaultGameModeBase.*   空の GameMode 基底クラス
  petitcon26.Build.cs     モジュールの依存関係
Config/                   Default*.ini
Content/                  アセット (.uasset / .umap。バイナリなので直接編集できない)
  DefaultMap.umap         ゲームとエディタの起動マップ
  BP_DefaultGameMode      グローバルの既定 GameMode
  BP_Player / BP_DodgeBall / BP_Enemy   C++ クラスを継承した Blueprint
  WBP_Title               タイトル画面ウィジェット
  Input/                  IMC_Default, IA_Throw, IA_MouseLook (Enhanced Input)
  Camera/                 Gameplay Cameras のアセット (CA_/CR_/CDE_)
  Animation/              ABP_Player, AM_Throw, AS_Throw
  Environment/Gym/        体育館 (japanese_gymnasium) とそのマテリアル
  ControlRig/Characters/  UE 標準のマネキン (Manny/Quinn) 一式
  _GENERATED/             モデリングツールで生成したメッシュ
Cascadeur/                投球モーションの元データ (.casc, .fbx, 参考動画と画像)
```

## コードの構成

- **ADodgeBallPlayer**（`DodgeBallPlayer.h/.cpp`）
  - `BeginPlay` で `DefaultMappingContext` を Enhanced Input Subsystem に追加する。
  - 入力のバインドは C++ ではなく **Blueprint（BP_Player）側** で行い、`Look(FVector2D)` や `Shoot(FVector)` を呼び出す（どちらも `BlueprintCallable`）。
  - `Look` は視点を回転させたあと、`LimitAimAngle` でピッチを `AimLimitMin` から `AimLimitMax`（既定は ±45°）の範囲に制限する。
  - `Shoot` は `DodgeBallActor` を Spawn して `ADodgeBall::Shoot` を呼ぶ。そのあいだ手に持ったダミーボール（`StaticDummyDodgeBall`）を隠し、2 秒後にボールを消してダミーボールを再び表示する。
  - カメラには `UGameplayCameraComponent`（GameplayCameras プラグイン）を使う。
- **ADodgeBall**（`DodgeBall.h/.cpp`）
  - 物理シミュレーションを有効にした StaticMesh をルートにしている。`Shoot()` で前方にインパルスを加える（`30000` は仮の固定値で、TODO あり）。
- **AEnemy**（`Enemy.h/.cpp`）
  - Capsule、SkeletalMesh、肩と足の Sphere を持つ Pawn。
  - `NotifyActorBeginOverlap` で効果音（`AttackSound`）を鳴らし、`ActivateRagdoll` を呼んだあと `BlowAwayRagdoll`（放射状インパルス）で吹き飛ばす。3 秒後に Destroy する。
  - 敵を継続して出現させる処理は Blueprint かマップ側にある（C++ にはない）。
- `Build.cs` の依存モジュール: Core, CoreUObject, Engine, InputCore, EnhancedInput, GameplayCameras, PhysicsCore。新しい UE モジュールを使うときはここに追加する。
- 有効なプラグイン: ModelingToolsEditorMode（Editor のみ）, AIAssistant。

## コーディング規約

- UE の命名規則に従う（`A`/`U`/`F` の接頭辞、`TObjectPtr<>`、`UPROPERTY`/`UFUNCTION`）。クラスには `PETITCON26_API` を付ける。
- Blueprint から設定または呼び出すものには `EditAnywhere, BlueprintReadWrite` や `BlueprintCallable` を付けるのが基本。
- 遅延処理は `FTimerHandle` と `FTimerDelegate::BindLambda` で書いている。
- ログは `UE_LOG(LogTemp, ...)`。
- **コメントは日本語**で書く。新しいコメントも日本語にする。
- インデントはタブ。

## Git

- コミットメッセージは**日本語の短い一文**（例:「敵を継続して出現させ、ボールが当たった時に音が出るようにした」）。Issue に関係するときは先頭に `#番号` を付ける（例: `#4 手首の動きを修正した`）。
- `.uasset` / `.umap` はバイナリで、Git LFS なしで直接コミットされている。マージで競合しても解決できないので注意する。
- `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `*.sln`, `*_BuiltData.uasset` などは `.gitignore` で除外している。`Content/Locodrome/`, `Content/NiagaraExamples`, `Content/DissolveVFX` も除外していて、リポジトリには含まれない。

## 注意点

- アセットの中身（Blueprint グラフ、マップ上の配置、入力マッピングの詳細など）は、このリポジトリのテキストからは読めない。C++ 側の変更が Blueprint の設定に依存するときは、その依存をユーザーに伝える。
- `ADodgeBallPlayer` のコンストラクタは `SocketNameForBallHandling` を使ってダミーボールをアタッチしている。ただ、この値は Blueprint で設定するので、コンストラクタの時点では空になっている可能性がある。
