# ti-dh-php-c-extension
C语言扩展版本的PHP DH库，对PHP版本要求为>=7.0

## API说明：

### dh_init()函数
参数：无需任何参数
返回：PHP Array

| 字段 | 含义 |
| ------------ | ------------ |
| p |服务端计算出来的p，返回给客户端 |
| g |服务端计算出来的g，返回给客户端 |
| server_number |服务端【私钥】，请保密，不可以外泄 |
| processed_server_number |处理过的服务端【私钥】，返回给客户端 |


### dh\_compute\_share\_key()函数
原型：string dh\_compute\_share\_key( string client_number, string server_number, string p )
参数：

| 字段 | 含义 |
| ------------ | ------------ |
| client_number |客户端提交过来client_number |
| server_number |服务端server_number，未经过处理的需要保密的那个 |
| p |服务端计算出来的p |

输出：String，最终结果就是协商完成的用于对称加解密的密钥


## 安装指南：
1. git clone https://github.com/ti-dh/ti-dh-php-c-extension.git
2. cd ti-dh-php-c-extension，总之就是切换到代码根目录中
3. 输入命令phpize
4. 输入命令./configure
5. 输入命令make
6. 输入命令sudo make install
7. 最后是将安装好的dh.so配置到你的php.ini中，最后一步与你具体PHP环境有关


## DEMO运行：
1. 进入到代码根目录的example文件夹下
2. 执行命令 php -S 0.0.0.0:8877 server.php
3. 然后再打开一个新的example文件夹终端，执行命令 php client.php

