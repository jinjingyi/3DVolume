### 1、解决了内存问题

### 2、从切片上拾取点并计算在三维空间中的坐标

### 3、三维点标记

### 4、三维空间点的增删，设计测量模式的UI

### 5、添加了表格的数据增删

### 6、调试二次加载的BUG(添加重置功能)

### 7、裁剪功能的图像导入与滑条索引

### 8、优化：测量数据不可编辑

### 9、路径编码问题

### 10、实现裁剪功能(需要OpenCV)

### 11、增加导出裁剪图像时显示进度条的功能，由于VTK图像读取在内部完成，实现进度条功能相对复杂，暂未实现。

### 12、裸数据(二进制)裁剪

### 13、三维切面默认不显示

### 14、背景默认为黑色

### 15、bin文件读取增加自动读取config文件，确定宽高参数

### 16、裁剪后可直接读入到三维体绘制中，仍保留裁剪后保存的功能

### 17、三维体绘制的切面改为半透明

### 18、经测试，裁剪后读入(内存读入)出现了内存泄漏的问题，获取每一层的指针，解决了这个问题

### 19、解决进度条出现了假死的情况

### 20、解决选择文件夹为空的崩溃问题

### 21、实现对话框裁剪序列图像

### 22、在MainWindow中声明CropSequenceDialog为友元类，CropSequenceDialog则可以直接访问MainWindow的成员(包括私有)
