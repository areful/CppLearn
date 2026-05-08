
# MSYS2 Pacman 国内镜像稳定版（清华+中科大+北外 优先级排序，官方兜底）
## 一、三条配置文件直接替换
分别编辑这三个文件：
```
/etc/pacman.d/mirrorlist.mingw64
/etc/pacman.d/mirrorlist.mingw32
/etc/pacman.d/mirrorlist.msys
```

### 1. mirrorlist.mingw64 完整内容
```ini
# 国内镜像优先
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64
Server = https://mirrors.ustc.edu.cn/msys2/mingw/x86_64
Server = https://mirrors.bfsu.edu.cn/msys2/mingw/x86_64
# 官方兜底
Server = https://mirror.msys2.org/mingw/x86_64
Server = https://repo.msys2.org/mingw/x86_64
```

### 2. mirrorlist.mingw32 完整内容
```ini
# 国内镜像优先
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686
Server = https://mirrors.ustc.edu.cn/msys2/mingw/i686
Server = https://mirrors.bfsu.edu.cn/msys2/mingw/i686
# 官方兜底
Server = https://mirror.msys2.org/mingw/i686
Server = https://repo.msys2.org/mingw/i686
```

### 3. mirrorlist.msys 完整内容
```ini
# 国内镜像优先
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch
Server = https://mirrors.ustc.edu.cn/msys2/msys/$arch
Server = https://mirrors.bfsu.edu.cn/msys2/msys/$arch
# 官方兜底
Server = https://mirror.msys2.org/msys/$arch
Server = https://repo.msys2.org/msys/$arch
```

## 二、一键命令直接写入（不用手动改文件）
复制到 MSYS2 终端直接运行，自动覆盖成上面稳定配置：
```bash
# mingw64
cat > /etc/pacman.d/mirrorlist.mingw64 <<'EOF'
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/x86_64
Server = https://mirrors.ustc.edu.cn/msys2/mingw/x86_64
Server = https://mirrors.bfsu.edu.cn/msys2/mingw/x86_64
Server = https://mirror.msys2.org/mingw/x86_64
Server = https://repo.msys2.org/mingw/x86_64
EOF

# mingw32
cat > /etc/pacman.d/mirrorlist.mingw32 <<'EOF'
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/mingw/i686
Server = https://mirrors.ustc.edu.cn/msys2/mingw/i686
Server = https://mirrors.bfsu.edu.cn/msys2/mingw/i686
Server = https://mirror.msys2.org/mingw/i686
Server = https://repo.msys2.org/mingw/i686
EOF

# msys
cat > /etc/pacman.d/mirrorlist.msys <<'EOF'
Server = https://mirrors.tuna.tsinghua.edu.cn/msys2/msys/$arch
Server = https://mirrors.ustc.edu.cn/msys2/msys/$arch
Server = https://mirrors.bfsu.edu.cn/msys2/msys/$arch
Server = https://mirror.msys2.org/msys/$arch
Server = https://repo.msys2.org/msys/$arch
EOF
```

## 三、刷新源并升级
```bash
pacman -Syyu
```

## 四、特点
1. 清华→中科大→北外 依次轮询，哪个快用哪个
2. 国内源挂了自动切官方，**永不失效**
3. 后续安装 gcc、cmake、Qt6、openssl 等都秒速下载