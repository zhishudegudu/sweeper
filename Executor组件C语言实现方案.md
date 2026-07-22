# 扫地机迭代一实现方案

## 1. 目标

迭代一只实现以下需求：

1. Config 初始化扫地机的位置和朝向；
2. 默认位置为 `(0, 0)`，默认朝向为北；
3. Controller 发出右转、左转指令；
4. Executor 执行转向并保存当前状态；
5. Controller 查询当前坐标和朝向；
6. 提供测试用例。

Wifi、Clean、Alert、前进、后退等功能不在本次实现范围内。

## 2. 组件关系

```text
Config ----初始化----> Executor <----控制和查询---- Controller
```

- Config：提供初始坐标和朝向；
- Executor：保存状态，执行左右转；
- Controller：调用 Executor 的转向和查询接口。

## 3. 数据结构

朝向：

```c
typedef enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
} Heading;
```

Executor 保存当前位置和朝向：

```c
typedef struct {
    int x;
    int y;
    Heading heading;
} Executor;
```

Config 保存初始化参数：

```c
typedef struct {
    int x;
    int y;
    Heading heading;
} Config;
```

## 4. 主要接口

Config：

```c
void config_default(Config *config);
void config_init_executor(const Config *config, Executor *executor);
```

Executor：

```c
void executor_init(Executor *executor, int x, int y, Heading heading);
void executor_turn_right(Executor *executor);
void executor_turn_left(Executor *executor);
void executor_get_position(const Executor *executor,
                           int *x, int *y, Heading *heading);
```

Controller：

```c
void controller_turn_right(Executor *executor);
void controller_turn_left(Executor *executor);
void controller_get_position(const Executor *executor,
                             int *x, int *y, Heading *heading);
```

## 5. 转向规则

| 当前方向 | 右转后 | 左转后 |
|---|---|---|
| 北 | 东 | 西 |
| 东 | 南 | 北 |
| 南 | 西 | 东 |
| 西 | 北 | 南 |

转向只改变朝向，不改变 `x` 和 `y`。

## 6. 程序流程

```text
Config 生成默认配置 (0, 0, 北)
    ↓
Config 初始化 Executor
    ↓
Controller 发出左转或右转命令
    ↓
Executor 修改朝向
    ↓
Controller 查询坐标和朝向
```

## 7. 文件结构

```text
include/
├── config.h
├── controller.h
└── executor.h

src/
├── config.c
├── controller.c
├── executor.c
└── main.c

test/
├── test_iteration1.cpp
├── test_iteration2.cpp
└── test_iteration3.cpp
```

## 8. 测试用例

| 用例 | 预期结果 |
|---|---|
| Config 默认初始化 | `(0, 0, 北)` |
| 从北向右转 | `(0, 0, 东)` |
| 从东向左转 | `(0, 0, 北)` |
| 指定初始化 `(10, 20, 南)` | 查询结果为 `(10, 20, 南)` |

三个迭代的测试统一使用 GoogleTest，通过 `EXPECT_EQ` 和 `ASSERT_EQ`
表达预期结果，并由 CTest 统一运行。

## 9. 运行方式

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## 10. 迭代三：危险点告警

组件职责：

- Config 使用数组保存多个危险点，并保证它们与初始坐标不同；
- Config 拒绝重复危险点，初始化 Executor 时批量下发；
- Executor 每次执行命令后统一遍历危险点；
- 当前位置等于任一危险点时调用 `alert(IN_DANGEROUS, x, y)`；
- 在危险点反复执行命令时，每次都重新告警；
- Alert 组件只负责处理告警，不参与坐标判断。

调用流程：

```text
Config 添加多个危险点
    ↓
Config 初始化 Executor 并下发危险点
    ↓
Controller 发出命令
    ↓
Executor 执行命令并检查危险点
    ↓ 命中
Alert(type=3, x, y)
```

Executor 默认使用真实的 `alert()`，测试时通过 `executor_set_alert_handler()`
替换为记录函数，从而验证告警次数、类型和坐标，不把测试逻辑写入业务组件。
