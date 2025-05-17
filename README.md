# 离散数学大作业项目说明

## 作者信息

- **姓名**: 杨瑞晨
- **学号**: 2351050
- **邮箱**: <2351050@tongji.edu.cn>
<richardyangruichen@gmail.com>

## 题目信息

- **01** 命题逻辑连接词、真值表、主范式
- **02** 命题逻辑推理
- **03** 求关系的自反、传递和对称闭包
- **04** 最小生成树
- **05** 最优二元树的应用
- **06** Warshall 算法求关系的对称闭包

## 项目结构

每个项目的文件夹下包括项目的源码(.cpp)、说明文档(.pdf)、 windows 下可执行文件(.exe)与 linux 下可执行文件。

## 编译与运行

- linux 环境下使用 gcc 编译

```sh
g++ -o output_file source_file.cpp
```

- windows 环境下使用 mingw 编译，UTF-8编码

```sh
\mingw64\bin\g++.exe -o output_file.exe source_file.cpp
```

> 针对 windows 环境下，该项目的所有文件均已转为 GBK 编码，直接运行应该不会出现问题。若运行出现乱码，请将源文件通过 GBK 编码保存后编译生成可执行文件。
