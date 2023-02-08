# A* algorithm @ C lang

## 参考

### A*アルゴリズムについて
- yttm-work
    - https://yttm-work.jp/algorithm/algorithm_0015.html

- Wikipedia
    - https://ja.wikipedia.org/wiki/A*

### リストについて
- だえうホームページ
    - https://daeudaeu.com/node-structure/
- C言語によるはじめてのアルゴリズム入門　改訂第3版


#### その他メモ

- リスト利用について
    - C言語ではリスト構造を自力で作成する必要がある
    - ソートにおいては配列のほうがやりやすいことから、今回は配列で代替を行う

- 苦労点
    - 親をどうするか決めそこねた　アドレスを指定できなかった、ないし難しかった
    - xとyを間違えた
    - list型を作ったあと、変数名でlistを使ってしまい不具合が起きた
    - printf()で表示する変数を追加し忘れた
    - *hoge = fuga と &hoge = fuga の違いを理解しきれていなかった
    - 変数名を何回も間違えた

- 改良が可能そうな点
    - SetAdjacentNode()無いのpositionの値の変更はもっと簡単に書ける気がする
        - できた

- 多少無理してでも先にブーストしておくべきだった……