
# yolov5 bmruntime例程

使用sophon sdk的底层runtime c接口，搭建的串行的yolov5例程

## 目录结构
```
yolov5
├── cpp
│   ├── CMakeLists.txt
│   ├── main.cpp
│   ├── yolov5.cpp
│   └── yolov5.hpp
├── datasets          
│   ├── test
│   └── coco.names
├── models
│   └── BM1684X
└── readme.md
```

## 编译

```bash
cd cpp
mkdir build
cd build
cmake ..
make 
```

## 运行方式

参数说明：
```
Usage: yolov5_bmcv.pcie [params]

        --bmodel (value:../models/BM1684X/yolov5s_v6.1_3output_fp32_1b.bmodel)
                bmodel file path
        --dev_id (value:0)
                TPU device id
        --conf_thresh (value:0.5)
                confidence threshold for filter boxes
        --nms_thresh (value:0.5)
                iou threshold for nms
        --input (value:../datasets/test)
                input path, images direction or video file path
        --classnames (value:../datasets/coco.names)
                class names file path
        --help 
                print help information.

```
在cpp目录中，使用coco数据集作为输入：

```bash
./yolov5_bmcv.pcie --bmodel=../models/BM1684X/yolov5s_v6.1_3output_fp32_1b.bmodel --dev_id=0 --conf_thresh=0.5 --nms_thresh=0.5 --input=../datasets/test/ --classnames=../datasets/coco.names
```