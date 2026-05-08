# MSYS2 的 pacman 可用**清华、中科大、北外、南京大学**等国内镜像，速度明显优于官方源。

## 一、常用国内镜像地址
### 1. 清华大学（推荐）
- msys：`https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch/`
- mingw64：`https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64/`
- mingw32：`https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686/`

### 2. 中国科学技术大学
- msys：`https://mirrors.ustc.edu.cn/msys2/msys/$arch/`
- mingw64：`https://mirrors.ustc.edu.cn/msys2/mingw/x86_64/`

### 3. 北京外国语大学
- msys：`https://mirrors.bfsu.edu.cn/msys2/msys/$arch/`
- mingw64：`https://mirrors.bfsu.edu.cn/msys2/mingw/x86_64/`

---

## 二、快速配置（一键替换）
打开 MSYS2 终端，直接执行（以清华为例）：
```bash
sed -i "s#https\?://mirror.msys2.org/#https://mirrors.tuna.tsinghua.edu.cn/msys2/#g" /etc/pacman.d/mirrorlist*
```

## 三、手动修改（备选）
编辑 `/etc/pacman.d/` 下三个文件：
- `mirrorlist.msys`
- `mirrorlist.mingw64`
- `mirrorlist.mingw32`

在文件**最顶部**添加国内源，注释掉原有 `Server` 行（加 `#`）。

示例（mirrorlist.msys）：
```ini
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch/
# Server = https://mirror.msys2.org/msys/$arch/
# Server = https://repo.msys2.org/msys/$arch/
```

## 四、生效与更新
```bash
pacman -Syyu
```
- `-Syy`：强制同步数据库
- `-u`：升级所有包

## 五、验证
```bash
pacman -Ss curl
```
看输出的下载地址是否为 `mirrors.tuna.tsinghua.edu.cn` 等国内域名。

---
