# ti-dh-php-c-extension
C语言扩展版本的PHP DH库

### dh_compute_pga()函数
输入：不需要任何参数
输出：返回一个数组，数组内容由p,g,server_num,processed_server_num组成

### dh_compute_key()函数
输入：均为字符串类型，依次传参数p，server_num，client_num
输出：字符串，计算出来的用于对称加解密的key
