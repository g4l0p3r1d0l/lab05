# Laboratory work V — Юнит-тестирование и Mock-объекты

## Homework

Выполнение лабораторной работы №5 по созданию модульных тестов с использованием фреймворка Google Test и Mock-объектов Google Mock для банковской системы (классы `Account` и `Transaction`). Работа выполнена на операционной системе macOS.

---

### Шаг 1. Клонирование репозитория и настройка удаленного репозитория

1. Создание переменной окружения с токеном GitHub:
```bash
$ export GITHUB_TOKEN="my_token"

```

2. Переход в рабочую директорию и клонирование шаблона лабораторной работы:

```bash
$cd ~/workspace$ git clone [https://github.com/tp-labs/lab05](https://github.com/tp-labs/lab05) projects/lab05

```

Вывод команды:

```
Cloning into 'projects/lab05'...
remote: Enumerating objects: 137, done.
remote: Counting objects: 100% (25/25), done.
remote: Compressing objects: 100% (9/9), done.
remote: Total 137 (delta 18), reused 16 (delta 16), pack-reused 112 (from 1)
Receiving objects: 100% (137/137), 918.92 KiB | 2.10 MiB/s, done.
Resolving deltas: 100% (60/60), done.

```

3. Переход в папку проекта и перепривязка удаленного репозитория на личный аккаунт `g4l0p3r1d0l`:

```bash
$cd projects/lab05$ git remote remove origin
$ git remote add origin [https://github.com/g4l0p3r1d0l/lab05](https://github.com/g4l0p3r1d0l/lab05)

```

---

### Шаг 2. Создание структуры файлов проекта

1. Создание необходимых директорий для исходного кода домашней работы, тестов и воркфлоу GitHub Actions:

```bash
$mkdir -p homework/banking$ mkdir homework/tests
$ mkdir -p .github/workflows

```

2. Создание и наполнение конфигурационных файлов CMake и исходного кода (`Account.h`, `Account.cpp`, `Transaction.h`, `Transaction.cpp`) банковской системы:

```bash
$nano homework/CMakeLists.txt$ nano homework/banking/CMakeLists.txt
$nano homework/tests/CMakeLists.txt$ nano homework/banking/Account.h
$nano homework/banking/Account.cpp$ nano homework/banking/Transaction.h
$ nano homework/banking/Transaction.cpp

```

---

### Шаг 3. Подключение зависимости GoogleTest (gtest)

1. Создание папки для внешних зависимостей и добавление GoogleTest как Git-субмодуля:

```bash
$mkdir -p third-party$ git submodule add [https://github.com/google/googletest](https://github.com/google/googletest) third-party/gtest

```

Вывод команды:

```
Cloning into '/Users/mac/workspace/projects/lab05/third-party/gtest'...
remote: Enumerating objects: 28723, done.
remote: Counting objects: 100% (141/141), done.
remote: Compressing objects: 100% (101/101), done.
remote: Total 28723 (delta 80), reused 41 (delta 40), pack-reused 28582 (from 2)
Receiving objects: 100% (28723/28723), 13.86 MiB | 7.67 MiB/s, done.
Resolving deltas: 100% (21325/21325), done.

```

2. Фиксация стабильной версии субмодуля (`release-1.8.1`):

```bash
$ cd third-party/gtest && git checkout release-1.8.1 && cd ../..

```

Вывод команды:

```
Note: switching to 'release-1.8.1'.
You are in 'detached HEAD' state...
HEAD is now at 2fe3bd99 Merge pull request #1433 from dsacre/fix-clang-warnings

```

---

### Шаг 4. Решение проблем локальной конфигурации CMake

1. Первая попытка конфигурации CMake:

```bash
$ cmake -Hhomework -B_build -DBUILD_TESTS=ON

```

Вывод с ошибкой (неверное расположение папки `third-party` относительно корня CMake):

```
-- The C compiler identification is AppleClang 17.0.0.17000013
-- The CXX compiler identification is AppleClang 17.0.0.17000013
...
CMake Error at CMakeLists.txt:16 (add_subdirectory):
  add_subdirectory given source "third-party/gtest" which is not an existing
  directory.
-- Configuring incomplete, errors occurred!

```

2. Исправление путей — перенос `third-party` внутрь папки `homework` и повторный запуск:

```bash
$mv third-party homework/$ rm -rf _build
$ cmake -Hhomework -B_build -DBUILD_TESTS=ON

```

Вывод с ошибкой несовместимости старой версии gtest с новым компилятором/CMake:

```
CMake Error at third-party/gtest/CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 has been removed from CMake.
-- Configuring incomplete, errors occurred!

```

3. Обход ошибки совместимости добавлением флага политик `CMAKE_POLICY_VERSION_MINIMUM=3.5`:

```bash
$ cmake -Hhomework -B_build -DBUILD_TESTS=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5

```

Вывод с ошибкой отсутствия файлов тестов:

```
-- Configuring done (0.2s)
CMake Error at tests/CMakeLists.txt:1 (add_executable):
  Cannot find source file: test_account.cpp
CMake Generate step failed.  Build files cannot be regenerated correctly.

```

4. Исправление путей к файлам тестов в `homework/tests/CMakeLists.txt` и генерация исходников тестов:

```bash
$nano homework/tests/CMakeLists.txt$ find . -name "test_account.cpp"
$ cmake -Hhomework -B_build -DBUILD_TESTS=ON -DCMAKE_POLICY_VERSION_MINIMUM=3.5

```

Вывод успешной конфигурации:

```
-- Could NOT find PythonInterp (missing: PYTHON_EXECUTABLE) 
-- Configuring done (0.1s)
-- Generating done (0.0s)
-- Build files have been written to: /Users/mac/workspace/projects/lab05/_build

```

---

### Шаг 5. Компиляция проекта и адаптация под старый синтаксис GMock

1. Первая попытка компиляции:

```bash
$ cmake --build _build

```

Вывод с ошибкой компиляции (использование нового макроса `MOCK_METHOD`, не поддерживаемого версией 1.8.1):

```
/Users/mac/workspace/projects/lab05/homework/tests/test_transaction.cpp:12:23: error: unknown type name 'Lock'
   12 |      MOCK_METHOD(void, Lock, (), (override));
fatal error: too many errors emitted, stopping now [-ferror-limit=]
make[2]: *** [tests/CMakeFiles/check.dir/test_transaction.cpp.o] Error 1

```

2. Обновление кода тестов в `homework/tests/test_transaction.cpp` на совместимый синтаксис (`MOCK_METHOD0`, `MOCK_CONST_METHOD0`) и запуск финальной сборки:

```bash
$ cmake --build _build

```

Вывод успешной 100% компиляции:

```
[ 21%] Built target banking
[ 35%] Built target gtest
[ 50%] Built target gmock
[ 64%] Built target gmock_main
[ 78%] Built target gtest_main
[ 85%] Building CXX object tests/CMakeFiles/check.dir/test_transaction.cpp.o
1 warning generated.
[ 92%] Linking CXX executable check
[100%] Built target check

```

---

### Шаг 6. Локальный запуск юнит-тестов

Запуск тестов через утилиту `ctest` в развернутом (`--verbose`) режиме:

```bash
$ cd _build && ctest --verbose

```

Вывод результатов тестирования:

```
UpdateCTestConfiguration  from :/Users/mac/workspace/projects/lab05/_build/DartConfiguration.tcl
Test project /Users/mac/workspace/projects/lab05/_build
Constructing a list of tests
Done constructing a list of tests
Checking test dependency graph...
Checking test dependency graph end
test 1
    Start 1: check

1: Test command: /Users/mac/workspace/projects/lab05/_build/tests/check
1: Running main() from .../gtest_main.cc
1: [==========] Running 13 tests from 2 test cases.
1: [----------] Global test environment set-up.
1: [----------] 5 tests from Account
1: [ RUN      ] Account.Constructor
1: [       OK ] Account.Constructor (0 ms)
1: [ RUN      ] Account.ChangeBalanceThrowsIfNotLocked
1: [       OK ] Account.ChangeBalanceThrowsIfNotLocked (1 ms)
1: [ RUN      ] Account.ChangeBalanceWorksIfLocked
1: [       OK ] Account.ChangeBalanceWorksIfLocked (0 ms)
1: [ RUN      ] Account.DoubleLockThrows
1: [       OK ] Account.DoubleLockThrows (0 ms)
1: [ RUN      ] Account.UnlockWorks
1: [       OK ] Account.UnlockWorks (0 ms)
1: [----------] 5 tests from Account (1 ms total)
1: 
1: [----------] 8 tests from Transaction
1: [ RUN      ] Transaction.DefaultFee
1: [       OK ] Transaction.DefaultFee (0 ms)
1: [ RUN      ] Transaction.SetFee
1: [       OK ] Transaction.SetFee (0 ms)
1: [ RUN      ] Transaction.SameAccountThrows
1: [       OK ] Transaction.SameAccountThrows (0 ms)
1: [ RUN      ] Transaction.NegativeSumThrows
1: [       OK ] Transaction.NegativeSumThrows (0 ms)
1: [ RUN      ] Transaction.TooSmallSumThrows
1: [       OK ] Transaction.TooSmallSumThrows (0 ms)
1: [ RUN      ] Transaction.FeeTooBigReturnsFalse
1: [       OK ] Transaction.FeeTooBigReturnsFalse (0 ms)
1: [ RUN      ] Transaction.SuccessTransfer
1: 1 send to 2 $200
1: Balance 1 is 1000
1: Balance 2 is 499
1: [       OK ] Transaction.SuccessTransfer (0 ms)
1: [ RUN      ] Transaction.CallsAccountMethodsWithMock
1: 1 send to 2 $200
1: GMOCK WARNING: Uninteresting mock function call - returning default value.
1: [       OK ] Transaction.CallsAccountMethodsWithMock (0 ms)
1: [----------] 8 tests from Transaction (0 ms total)
1: 
1: [==========] 13 tests from 2 test cases ran. (1 ms total)
1: [  PASSED  ] 13 tests.
1/1 Test #1: check ............................   Passed    0.35 sec

100% tests passed, 0 tests failed out of 1

```

---

### Шаг 7. Синхронизация изменений с GitHub

1. Возврат в корень проекта, добавление файлов в коммит:

```bash
$cd ..$ git add .
$ git commit -m "feat: completed lab05 homework with 100% coverage"

```

Вывод команды:

```
[master defadd0] feat: completed lab05 homework with 100% coverage
 495 files changed, 175171 insertions(+)
 create mode 100644 .gitmodules
 create mode 100644 _build/CMakeCache.txt
 ...

```

2. Пуш изменений в ветку `master` удаленного репозитория:

```bash
$ git push origin master

```
