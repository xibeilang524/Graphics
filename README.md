# Graphics
>`2016年修改记录`
>>* 2016-08-30:建立初始版本
>>* 2016-08-31:具备删除、添加、置顶功能  
>>* 2016-08-31:增加缩放功能
>>* 2016-09-01:实现连线
>>* 2016-09-02:修改item继承备份
>>* 2016-09-02:完成不同类型的添加
>>* 2016-09-05:添加样式改变功能
>>* 2016-09-07:添加文字
>>* 2016-09-07:添加箭头
>>* 2016-09-07:添加delete删除事件
>>* 2016-09-08:添加类的描述信息
>>* 2016-09-08:修复单独删除箭头奔溃问题
>>* 2016-09-08:添加剪切—-复制-粘贴
>>* 2016-09-08:控件添加text显示，支持样式设置
>>* 2016-09-08:添加控件拖动位置实时显示
>>* 2016-09-09:增加文件保存、打开、格式验证
>>* 2016-09-09:支持对箭头的保存和解析
>>* 2016-09-09:添加对控件深度的记录和解析
>>* 2016-09-12:初步实现了对控件的缩放
>>* 2016-09-13:实现控件8角缩放，旋转后还未实现缩放
>>* 2016-09-13:实现对目前控件8个角拖拽
>>* 2016-09-13:修复本地打开和保存显示不一致问题
>>* 2016-09-14:修复复制后未选中控件，但复制按钮可用问题
>>* 2016-09-14:修复左右旋转90度导致旋转出错问题
>>* 2016-09-14:控件旋转修复阶段1-旋转角度还有问题
>>* 2016-09-18:控件支持端口拖入，但未支持连线、删除
>>* 2016-09-19:实现左侧控件树拖拽，图标还需修改
>>* 2016-09-19:修复复制尺寸没有复制问题
>>* 2016-09-19:添加对节点编辑窗口
>>* 2016-09-19:端口右键菜单每个对象独有
>>* 2016-09-19:修复程序退退出不完全；添加端口连线、删除功能
>>* 2016-09-19:剪切、复制支持端口
>>* 2016-09-19:实现左侧控件树拖拽，但端口无法拖拽至控件上
>>* 2016-09-20:修复拖入端口放置在控件上无法捕获bug
>>* 2016-09-20:调整工具栏，将控件点击创建统一改成拖入创建
>>* 2016-09-20:添加端口箭头绘制
>>* 2016-09-20:修改端口箭头指向末端边的交点
>>* 2016-09-21:修复复制、剪切Item时，ID号未更新问题;增加对节点的保存和加载
>>* 2016-09-21:端口加线段可以保存
>>* 2016-09-21:本地打开和保存基本完成完成_还需进一步测试
>>* 2016-09-21:本地打开和保存基本完成完成_修复保存再打开奔溃问题
>>* 2016-09-21:切换分支修复文字无法换行输入问题
>>* 2016-09-21:修复添加文字后无法换行、及输入卡顿问题
>>* 2016-09-22:修复移动控件后，修改样式位置发生微小偏移
>>* 2016-09-22:添加控件状态锁定、解锁功能
>>* 2016-09-22:对MainWindow和MyGraphicsView进行了重构，使得两者功能相对独立
>>* 2016-09-22:宏替换typeid().name()
>>* 2016-09-22:添加view拖拽、清空粘贴板
>>* 2016-09-22:准备添加撤销功能_已经添加至工具栏
>>* 2016-09-23:设置撤销状态初始不可用
>>* 2016-09-23:添加端口四个边任意移动功能
>>* 2016-09-23:增加Ctrl+滚轮缩小视图
>>* 2016-09-25:添加控件可显示隐藏
>>* 2016-09-25:添加锁定/解锁控件快捷键(Ctrl+L/Ctrl+U)
>>* 2016-09-25:增加窗口和帮助工具栏，支持全屏、左右栏快捷键操作
>>* 2016-09-27:调整左侧控件栏，调整拖入控件的尺寸
>>* 2016-09-27:添加平行四边形，修复菱形属性改变时形状改变
>>* 2016-09-27:修复控件输入文字后计算文字尺寸错误问题
>>* 2016-09-27:箭头添加文字，但还未保存
>>* 2016-09-28:线段添加文字可以保存和打开
>>* 2016-09-28:增加随父控件旋转角度变化，动态切换悬停的鼠标样式
>>* 2016-09-28:添加对箭头类型的切换
>>* 2016-09-28:绘制线段时增加箭头模式
>>* 2016-09-29:修复控件添加直线，由于鼠标点击在Item的Text上，导致无法添加新直线问题
>>* 2016-09-29:添加循环上限、循环下限、并行、注释
>>* 2016-09-29:修复控件添加直线，由于鼠标点击在Item的Text上，导致无法添加新直线问题
>>* 2016-09-30:调整控件四角不按比例缩放，取消四边中点缩放;增加四边连接线拖拽点
>>* 2016-09-30:调整item子控件的父类成QGraphicsPolygonItem
>>* 2016-10-02:添加显示item位置和旋转信息MyItemInfo
>>* 2016-10-03:修复因添加MyItemInfo导致清空，使程序奔溃问题
>>* 2016-10-03:调整平行四边形、注释、并行控件中心连接线显示位置
>>* 2016-10-03:实现多工作区切换,目前删除还存在问题
>>* 2016-10-04:修复关闭窗口奔溃问题
>>* 2016-10-05:多文档功能完善
>>* 2016-10-05:实现控件旋转【重大更新】
>>* 2016-10-08:修改控件旋转鼠标样式;添加控件连接线拖入时对当前鼠标下控件的检测
>>* 2016-10-08:完成直线从控件每边中点拖拽产生
>>* 2016-10-09:添加属性编辑框
>>* 2016-10-10:属性编辑框增加ModelView,性暂未保保存、未拷贝
>>* 2016-10-10:修复添加代理后，表格其它几列不能编辑问题
>>* 2016-10-10:零星功能修复;准备添加流程推演前
>>* 2016-10-10:初步实现对流程图的解析，待进行推演
>>* 2016-10-17:实现简版的推演功能;修复控件保存-打开-保存多次，线条上的文字信息重复保存;修复本地打开文件时，默认显示控件的信息问题
>>* 2016-10-17:修复新建工作区插入在前的问题;修复工具栏左旋90度角度为负问题;修复控件一直缩小导致变形问题
>>* 2016-10-18:修复切换工作模式后，显示/隐藏右侧推演栏无效问题
>>* 2016-10-18:添加对服务数据库的访问
>>* 2016-10-18:添加对服务数据库的访问_补充
>>* 2016-10-18:修复旋转后尺寸缩放的问题;修复本地打开后，拖拽点鼠标样式不随角度改变问题
>>* 2016-10-20:修改服务的属性信息，增加从数据库读取信息;增加窗口退出提示
>>* 2016-10-20:修复工具栏名称切换不一致;程序增加图标;修复mingw编译器报的警告
>>* 2016-10-20:添加打开文件与当前版本的比对
>>* 2016-10-21:修复删除item后，清空和本地保存按钮仍然可用问题;修复产生连接线时，粘贴按钮被置为可用问题;
>>* 2016-10-21:添加工作区切换后，工具栏依照相应的控件来自动的更改状态
>>* 2016-10-25:增加从过程列表直接双击显示处理单元属性编辑信息;增加Alt+左右键快速切换工作区间;修复因流程不完整导致推演检查时程序奔溃问题
>>* 2016-10-25:修复从同一控件多次删除连线后，导致奔溃问题
>>* 2016-10-25:增加全局键盘捕获，针对Alt+左右键切换
>>* 2016-10-25:增加工作区间右键菜单，支持关闭、保存、关闭左侧所有、关闭右侧所有
>>* 2016-10-26:修复工作区间个数为0时，依然可以响应Ctrl+w的事件;修复工作区切换键盘事件多次响应问题;调整弹出提示框至Util;添加对数据库错误信息提示;
>>* 2016-10-26:添加确保右键菜单只选中一个控件;修复旋转控件时，因刷新不及时导致线条显示重叠问题
>>* 2016-10-27:添加D形和三角形端口
>>* 2016-10-27:修复因多个判断未完全处理结束而导致流程自动退出
>>* 2016-10-27:添加圆形端口
>>* 2016-10-28:调整右键菜单至统一管理;端口添加文字
>>* 2016-10-28:添加条件输入框
>>* 2016-10-31:调整服务编辑输入输出参数从从数据库读取;调整只显示可用状态的服务
>>* 2016-11-01:增加GRA_RECT引单功能
>>* 2016-11-04:添加对for和while循环的支持;
>>* 2016-11-04:增加禁止从正在模拟的状态退出至建模状态
>>* 2016-11-06:添加对doWhile循环的支持【！！doWhile循环和是否判断会重合】
>>* 2016-11-07:修复因dowhile产生的rect无限循环问题(尚未对判断框功能的区分(判断/循环))
>>* 2016-11-07:修复提取出的处理单元部分判断节点左右子节点为null，导致访问出错问题
>>* 2016-11-08:添加状态机支持，尚未添加关联
>>* 2016-11-10:调增分离控件右击和双击的事件响应;添加对复合模型组件图支持;
>>* 2016-11-10:添加软件关于页面
>>* 2016-11-10:增加服务推演;增加推演过程中引用前者的输出作为自己的输入
>>* 2016-11-14:增加对循环、嵌套循环的支持(目前仅支持一个条件循环判断)
>>* 2016-11-15:增加对服务、循环属性的保存;修复计算循环--条件错误问题
>>* 2016-11-15:添加自动/单步运行
>>* 2016-11-17:增加对循环或判断控件的引单功能;增加循环通过引单计算循环
>>* 2016-11-17:增加推演高亮提示(用于指示当前推演的进度);增加对循环过程的记录，支持点击右侧记录查看当前循环的值
>>* 2016-11-21:添加文件保存时对版本号的写入;添加对判断框条件引用的解析，实现第一步对条件的去引用
>>* 2016-11-22:添加状态机词法分析判断框条件
>>* 2016-11-23:添加对判断条件执行，实现判断功能
>>* 2016-11-23:修复因未深度拷贝，导致多次打开窗口，访问输入参数出错问题
>>* 2016-11-25:增加单步推演查看信息时禁止编辑;调整参数引用只引用上一步;增加属性编辑框查左侧查看上步输出结果
>>* 2016-11-30:修改对服务输出的解析，支持多维数组
>>* 2016-11-30:添加输入输出框的显示和文件保存功能
>>* 2016-12-01:增加文件拖入打开;增加窗口标题显示打开文件信息;增加对全路径名一致文件判断，确保只打开一次
>>* 2016-12-01:添加快捷键显示列表
>>* 2016-12-09:建立折线初始版本，但折线还不智能
>>* 2016-12-09:添加常用几种折线状态，还不完善
>>* 2016-12-10:调整若菱形的no分支指向前面的控件，则将菱形认定为判断框而非dowhile循环
>>* 2016-12-11:折线添加文字;修改折线外边框区域矩形
>>* 2016-12-11:流程推演添加折线
>>* 2016-12-11:添加程序启动页面
>>* 2016-12-12:完成折线16种状态的搭配，暂不支持旋转
>>* 2016-12-12:修复创建原子图时自动创建的文件名未添加版本后缀问题
>>* 2016-12-16:添加推演前统一面板设定
>>* 2016-12-16:端口添加折线
>>* 2016-12-20:完成在流程前后加入预处理和环境恢复功能
>>* 2016-12-20:添加服务调用前后对服务状态的更改
>>* 2016-12-23:增加D型输入端口
>>* 2016-12-26:添加设置折线两端点箭头类型
>>* 2017-01-03:修改初始化服务根据服务名称循环URL
>>* 2017-02-25:16年完结版<br>
>`2017年修改记录`<br>
>>* 2017-02-28:添加对预处理服务的本地保存和加载;修复双层dowhile循环嵌套失败问题;
>>* 2017-03-01:修复编辑预处理服务时没有访问路径的问题
>>* 2017-03-07:完成端口仿真属性的保存和加载
>>* 2017-03-08:完成对端口信息信息添加和保存
>>* 2017-03-08:完成状态图中处理框属性的添加和保存
>>* 2017-03-09:添加仿真状态连线的属性引用和保存
>>* 2017-03-13:添加外部启动程序入口，支持启动参数设置
