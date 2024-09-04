#include <iostream>
#include "yolov5.hpp"


int main(int argc, char *argv[]){
  const char *keys="{bmodel | ../models/BM1684X/yolov5s_v6.1_3output_fp32_1b.bmodel | bmodel file path}"
  "{dev_id | 0 | TPU device id}"
  "{conf_thresh | 0.5 | confidence threshold for filter boxes}"
  "{nms_thresh | 0.5 | iou threshold for nms}"
  "{help | 0 | print help information.}"
  "{input | ../datasets/test | input path, images direction or video file path}"
  "{classnames | ../datasets/coco.names | class names file path}"
  "{use_cpu_opt | false | accelerate cpu postprocess}";
  
  cv::CommandLineParser parser(argc, argv, keys);
  if (parser.get<bool>("help")) {
    parser.printMessage();
    return 0;
  }

  std::string bmodel_path = parser.get<std::string>("bmodel");
  std::string input_path = parser.get<std::string>("input");
  std::string coco_names = parser.get<std::string>("classnames");

  int dev_id = parser.get<int>("dev_id");
  float conf_thresh = parser.get<float>("conf_thresh");
  float nms_thresh = parser.get<float>("nms_thresh");

  std::filesystem::path dirPath("results/images");
  std::filesystem::create_directories(dirPath);
  
  auto yolov5 = YOLOv5(dev_id, bmodel_path, conf_thresh, nms_thresh, coco_names);

  int batch_size = yolov5.get_batch_size();


  // image dir
  std::vector<std::string> image_paths;
  for (const auto& entry: std::filesystem::directory_iterator(input_path)){
    if (entry.is_regular_file()){
      image_paths.emplace_back(entry.path().filename().string());
    }
  }

  int image_num = image_paths.size();
  std::sort(image_paths.begin(), image_paths.end());

  std::vector<cv::Mat> batch_imgs;
  std::vector<std::string> batch_names;
  std::vector<YoloV5BoxVec> boxes;
  int id = 0;

  for (auto iter = image_paths.begin(); iter != image_paths.end(); iter++){
    id += 1;
    std::string img_file = *iter;
    std::cout << id << "/" << image_paths.size() << ", img_file: " << img_file << std::endl;
    cv::Mat image = cv::imread(input_path+"/"+img_file, cv::IMREAD_COLOR, dev_id);
    batch_imgs.emplace_back(image);
    batch_names.emplace_back(img_file);

    if (batch_imgs.size() == batch_size){
      yolov5.yolov5_detect(batch_imgs, boxes);
      for (int i = 0; i < batch_size; i++){
        for (auto bbox: boxes[i]){
          yolov5.yolov5_draw(bbox.class_id, 
                            bbox.score, 
                            bbox.x, 
                            bbox.y, 
                            bbox.width+bbox.x,
                            bbox.height+bbox.y,
                            batch_imgs[i]);

        }
        std::string save_path = "results/images/" + batch_names[i];
        cv::imwrite(save_path, batch_imgs[i]);
      }
      batch_imgs.clear();
      batch_names.clear();
      boxes.clear();
    }

  }



  std::cout << std::endl;

  
}