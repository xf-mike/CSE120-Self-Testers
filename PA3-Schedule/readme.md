# 内核模拟测试框架

这个测试框架是一个对内核的模拟。  
简单来说，我们不关注每个进程在做什么，  
我们只是将内核操作抽象出来，  
通过跟踪内核操作本身而不是 `DPrintf` 打印，  
从而考察我们自身实现的接口是否正确。  

我们只需要一个进程就可以模拟内核与多个进程的交互。  

## 如何运行

你可以试着先运行 `t`（`t` 是 `test` 的缩写），  
然后会有一个 `testbase.c` 文件生成。  
它是由你的 `mycode.c` 经过一些字符串处理，  
再注入 `injector.c`，最后形成的。  
你看一眼就能理解，很简单。  

我们编写的测试用例（test case）就是基于 `testbase.c`。  

所有的测试用例都只应当调用 `injector.c` 中的接口，  
从而模拟内核操作。  

## 如何修改测试用例

你可以修改 `test.c` 中 `main` 函数调用的测试用例，  
然后重新运行 `t`（直接在终端输入 `t` 并回车即可）。  
对应的测试日志（log）在 `log` 文件夹中。  

你的输出可能跟我的不一样，  
因为每个人的实现都会有一些区别。  

## 重要说明

- **默认情况下**，你在 `mycode.c` 中的 `DPrintf` 会被 **mute**。  
  如果你需要原本的输出，你可以加 `verbose` 标志，运行：

  ```sh
  t -v
  ```

- **你也可以自己写测试样例**。  
  `test.c` 中有很多例子，先看一遍，自然就会写了。

## 贡献与支持

如果你觉得我的日志无法理解或者发现了错误，  
**请提交 PR**！  

如果你觉得我的框架对你有帮助，  
**请给我一个 Star** ⭐！  

## 特别感谢

最后，我想特别感谢 **ygong15**（[GitHub](https://github.com/ygong15)）。  
没有他的帮助，我也很难构建出这样细致的测试用例。  

---

# Kernel Simulation Testing Framework

This testing framework is a simulation of the kernel.  
Simply put, we don't focus on what each process is doing.  
Instead, we abstract kernel operations and examine whether our own implementation interfaces are correct by tracking the kernel operations themselves rather than using `DPrintf` prints.  

We only need a single process to simulate the interaction between the kernel and multiple processes.  

## How to Run

You can try running `t` first (where `t` is short for `test`).  
This will generate a `testbase.c` file, which is derived from your `mycode.c` through some string processing.  
It is then injected into `injector.c` to form the final result.  
Take a quick look—it's simple and easy to understand.  

The test cases we write are based on `testbase.c`.  

All test cases should only call the interfaces in `injector.c` to simulate kernel operations.  

## Modifying Test Cases

You can modify the test cases called in the `main` function of `test.c` and then rerun `t`  
(just type `t` in the terminal and press Enter).  
The corresponding test logs can be found in the `log` folder.  

Your output might be different from mine because everyone's implementation may vary slightly.  

## Important Notes

- **By default**, `DPrintf` in `mycode.c` is **muted**.  
  If you need the original output, you can add the `verbose` flag and run:

  ```sh
  t -v
  ```

- **You can also write your own test cases**.  
  There are many examples in `test.c`—just take a look, and you'll quickly understand how to write them.

## Contribution & Support

If you find my logs hard to understand or notice any errors,  
**please submit a PR**!  

If you find my framework helpful,  
**I would appreciate a Star** ⭐!  

## Special Thanks

Lastly, I would like to give special thanks to **ygong15** ([GitHub](https://github.com/ygong15)).  
Without his help, it would have been much harder to construct such detailed test cases.  

---

**Mike F**