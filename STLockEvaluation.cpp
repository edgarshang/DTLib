#include "STLockEvaluation.h"
#include "sense_atsha204a.h"
#include "st_face.h"
#include "st_face_module.h"
#include "sense_log.h"
#include "common.h"

#include <unistd.h>
#include <fstream>
#include <sstream>
#include <string>
#include <dirent.h>
#include <sys/stat.h>
#include <map>
#include <memory>



#define LOG_TAG "STLockEvaluation"


EvaConfig Config= {
    .evalutaion_data_dir = "/mnt/sdcard/pick_test",
    .result_report_dir = "/mnt/sdcard/report",
    .feature_dir = "/mnt/sdcard/feature",
    .pixel_format = ST_PIX_FMT_GRAY8,
    .image_width = 640,
    .image_height = 400,
    .face_orientation = ST_FACE_LEFT
};



#define TEMPLATE_PATH "/data"
st_module_config_t st_lock_module_config =
{
    .template_save_path = (char*)TEMPLATE_PATH,
    .resize_image = ST_FACE_DEFAULT_CONFIG,
    .calc_pose = nullptr,
    .face_rect = nullptr,
    .algo_abort = nullptr
};

st_result_t STLockEvaluation::InitSdk()
{
    char* license = nullptr;
    license = get_license_info();

    int ret_license = st_face_init_license(license);

    release_license_buffer(license);
    if (ST_OK != ret_license) {
        LOGD("license auth failed");
        return INVALID_LICENSE;
    }

    int sdk_init_result = st_face_module_init(&st_lock_module_config);
    if (ST_OK != sdk_init_result) {
        LOGE("sdk init failed, sdk_init_result = %d", sdk_init_result);
        return UNKNOWN_ERROR;
    }

    return ST_OK;
}

STLockEvaluation::STLockEvaluation()
{

}

STLockEvaluation::STLockEvaluation(EvaConfig& config):evaConfig_(config)
{

}
STLockEvaluation::~STLockEvaluation()
{
    st_face_module_deinit();
    LOGD("release features");
}


st_result_t STLockEvaluation::WriteHeader(std::string file_path, std::string &content)
{
    std::fstream verify_f(file_path, std::ios::out);
 
    if (!verify_f.is_open()) {
        return -1;
    }

    std::stringstream ss;
    ss << content << std::endl;
    std::string log_str = ss.str();
    verify_f.write(log_str.c_str(), log_str.size());
    verify_f.close();
    return ST_OK;
}

st_result_t STLockEvaluation::ReportResult()
{
    std::vector<std::string>hacknessfrr;
    std::vector<std::string>hacknessfar;

    std::vector<std::string>verifyfrr;
    std::vector<std::string>verifyfar;

    std::string hackfarfile(HACK_FAR_PATH);
    std::string hackfrrfile(HACK_FRR_PATH);

    std::string verifyfarfile(VERIFY_FAR_PATH);
    std::string verifyfrrfile(VERIFY_FRR_PATH);

    readhackfile(hackfarfile, hacknessfar);
    readhackfile(hackfrrfile, hacknessfrr);
    readverifyfile(verifyfarfile, verifyfar);
    readverifyfile(verifyfrrfile, verifyfrr);

    double resFar = 0;
    double resFrr = 0;
    std::stringstream ss;
    std::vector<std::string>filestring;

    for(size_t i = 0; i < 54; i++)
    {
        resFar = cal(ThreSholds[i], hacknessfar, FAR, LIVENESS);
        resFrr = cal(ThreSholds[i], hacknessfrr, FRR, LIVENESS);
        ss << ThreSholds[i] << "," << resFrr << "," << resFar;
        filestring.push_back(ss.str());
        ss.str("");
    }

    WriteStrings(filestring, HACK_SUM_PATH);
    filestring.clear();

    for(size_t i = 0; i < 54; i++)
    {
        resFar = cal(ThreSholds[i], verifyfar, FAR, VERIFY);
        resFrr = cal(ThreSholds[i], verifyfrr, FRR, VERIFY);
        ss << ThreSholds[i] << "," << resFrr << "," << resFar;
        filestring.push_back(ss.str());
        ss.str("");
    }

    WriteStrings(filestring, VERIFY_SUM_PATH);
    filestring.clear();

    return ST_OK;
}

double STLockEvaluation::cal(double threhold, std::vector<std::string>&score, char status, char type)
{
    int molecule = 0;
    int Denominator = 0;
    for(size_t i = 0; i < score.size(); i++)
    {
        Denominator++;
        switch (status)
        {
            case FRR:
            {
                if(VERIFY == type )
                {
                    if (threhold > atof(score[i].c_str())) 
                    {
                        molecule++;
                    }
                }else if (LIVENESS == type)
                {
                    if (threhold < atof(score[i].c_str())) 
                    {
                        molecule++;
                    }
                } 
            }
            break;
        
            case FAR:
            {
                if(VERIFY == type)
                {
                    if (threhold < atof(score[i].c_str())) 
                    {
                        molecule++;
                    }
                }else if (LIVENESS == type) 
                {
                    if (threhold > atof(score[i].c_str())) 
                    {
                       molecule++;
                    }
                }
            }
            break;

            default:
                break;
        }
    }

    double result = (double)molecule / (double)Denominator;

    return result;
}


st_result_t STLockEvaluation::InitResultDir()
{
     if (!createDir(HACK_DIR))
    {
        LOGD("create dir %s faile\n", HACK_DIR);
        return -1;
    }

     if (!createDir(VERIFY_DIR)) 
    {
        LOGD("");
        return -1;
    }

  std::string filepath = std::string(HACK_DIR) + "/" + std::string(HACK_FAR);
  std::string header = "Path,livesss";
  WriteHeader(filepath, header);

  filepath = std::string(HACK_DIR) + "/" + std::string(HACK_QUA_DETECT);
  header = "Path, top, boom, left, right, liveness, verify, quality";
  WriteHeader(filepath, header);

  filepath = std::string(HACK_DIR) + "/" + std::string(REAL_QUA_DETECT);
  header = "Path, top, boom, left, right, liveness, verify, quality";
  WriteHeader(filepath, header);


  filepath = std::string(HACK_DIR) + "/" + std::string(HACK_FRR);
  header = "Path,livesss";
  WriteHeader(filepath, header);

  filepath = std::string(HACK_DIR) + "/" + std::string(HACK_SUM);
  header = "Thresholds, Hackness_FRR, Hackness_FAR";
  WriteHeader(filepath, header);


  filepath = std::string(VERIFY_DIR) + "/" + std::string(VERIFY_FAR);
  header = "origin_raw_path, verify_raw_path, verifyScore";
  WriteHeader(filepath, header);


  filepath = std::string(VERIFY_DIR) + "/" + std::string(VERIFY_FRR);
  header = "origin_raw_path, verify_raw_path, verifyScore";
  WriteHeader(filepath, header);

  filepath = std::string(VERIFY_DIR) + "/" + std::string(VERIFY_SUM);
  header = "Thresholds, Verify_FRR, Verify_FAR";
  WriteHeader(filepath, header);


  return 0;
}

bool STLockEvaluation::getEnrollPath(std::string single_root_path, std::vector< std::string > &enroll_paths)
{
    std::string enroll_root_path = single_root_path + "/continue/enroll";
    LOGI("enroll path : %s ", enroll_root_path.c_str());
    return getFileList(enroll_root_path.c_str(), enroll_paths);
}

bool STLockEvaluation::getRealPath(std::string single_root_path, std::vector< std::string > &real_paths)
{
    std::string enroll_root_path = single_root_path + "/continue/live";
    LOGI("real path : %s ", enroll_root_path.c_str());
    return getFileList(enroll_root_path.c_str(), real_paths);
}

bool STLockEvaluation::getHackPath(std::string single_root_path, std::vector< std::string > &hack_paths)
{
    std::string enroll_root_path = single_root_path + "/continue/hack";
    LOGI("hack path : %s ", enroll_root_path.c_str());
    return getFileList(enroll_root_path.c_str(), hack_paths);
}

st_result_t STLockEvaluation::WriteStrings(std::vector< std::string > &strs, std::string path)
{
    
    if (strs.empty()) {
        return ST_OK;
    }
    std::fstream logf(path, std::ios::app | std::ios::out);
    if (!logf.is_open()) {
        return -1;
    }
    std::stringstream ss;
    for (auto &str : strs) {
        ss << str << std::endl;
    }
    std::string log_str = ss.str();
    logf.write(log_str.c_str(), log_str.size());
    logf.close();
    return ST_OK;
}




st_result_t STLockEvaluation::ExtractFaceInfo(std::vector< std::string > &allSingleDirLists)
{
    st_result_t ret;
    bool bRet(true);
    for (auto &single_dir : allSingleDirLists) 
    {
        LOGD("single_dir = %s\n", single_dir.c_str());
        int i = single_dir.find_first_of(evaConfig_.evalutaion_data_dir.c_str());
        std::string heard = single_dir.substr(i+evaConfig_.evalutaion_data_dir.size() + 1);

        std::string dtsDir = evaConfig_.result_report_dir + "/" + heard;

        // 存储每张图片的feature
        std::string feature_Dir = evaConfig_.feature_dir + "/" + heard;

        std::vector< std::string > real_paths;
        std::vector< std::string > hack_paths;
        std::vector< std::string > enroll_paths;

        bRet = getEnrollPath(single_dir, enroll_paths);
        if(true != bRet)
        {
            LOGD("getSingleEnrollPath error\n");
            return -1;
        }

        bRet = getRealPath(single_dir, real_paths);
        if(true != bRet)
        {
            LOGD("getSingleRealPath error\n");
            return -1;
        }

        bRet = getHackPath(single_dir, hack_paths);
        if(true != bRet)
        {
            LOGD("getSingleHackPath error\n");
            return -1;
        }

        LOGI("size : real %d  hack %d  enroll %d", real_paths.size(), hack_paths.size(), enroll_paths.size());

        std::string pathLeft;
        std::string pathRight;
        char *feature = nullptr; 
        int enrollLen; 
        int status;
        std::stringstream ssfeature;

        for(int i = 0; i < enroll_paths.size()/4; i++)
        {
            pathLeft = single_dir + "/continue/enroll/" + LEFTIMAGE + std::to_string(i) + FILETYPE;
            pathRight = single_dir + "/continue/enroll/" + RIGHTIMAGE + std::to_string(i) + FILETYPE;

            ret = Get_Enrollfeature(pathLeft, pathRight, &feature, &enrollLen, &status);
            if(ret != 0 || status != 0 )
            {
                LOGD("Get_Enrollfeature error..ret = %d, status = %d\n", ret, status);
            }else
            {
                // LOGD("Get_Enrollfeature right.. and enrollLen = %d\n", enrollLen);
                // 将enroll feature存起来，后续和本人real图verify计算frr
                enroll_data_.push_back((std::string(feature)));
                // 将enroll feature存到文件中，后续和其他人real图计算far
                ssfeature << pathLeft << " " << feature;
                feature_data.push_back(ssfeature.str());
                ssfeature.str("");
            }
            
        }
        // enroll feature写到对应的文件保存
        WriteFeatureFile(feature_Dir, ENROLLFEATURE, feature_data);
        feature_data.clear();

        
      
        float verify_core = 0.0;
        float liveness_score = 0.0;
        float quality = 0;
        st_rect_t ImageRect;
        feature = nullptr;
                   

        std::stringstream ssquality;
        std::stringstream ssrealhackness;
        ssfeature.str("");

        std::vector<std::string> liveFileFeature;
        std::vector<std::string> livehackness;
        std::vector<std::string> ssqualitypath;

        // 遍历real图
        for(int i = 0; i < real_paths.size()/4; i++)
        {
            pathLeft = single_dir + "/continue/live/" + LEFTIMAGE + std::to_string(i) + FILETYPE;
            pathRight = single_dir + "/continue/live/" + RIGHTIMAGE + std::to_string(i) + FILETYPE;
            ret = GetPathFeature(pathLeft, pathRight, &feature, &enrollLen, &ImageRect, &liveness_score, &status, &quality);
            if(ret != 0 || status != 0 )
            {
                LOGD("getRealPathInfoImage error..ret = %d, status = %d\n", ret, status);
                
            }else
            {
                // 存储live图的feature
                ssfeature << pathLeft << " " << feature;
                liveFileFeature.push_back(ssfeature.str());
                ssfeature.str("");
                // 存储live图的hackness
                ssrealhackness << pathLeft << "," << liveness_score;
                livehackness.push_back(ssrealhackness.str());
                ssrealhackness.str("");
                // live图和enroll图比较veirfy
               
                if(compare_feature(enroll_data_, feature, &verify_core) == ST_OK )
                {
                    ssquality << pathLeft << "," << ImageRect.top << "," << ImageRect.bottom << "," << ImageRect.left << "," << ImageRect.right << ","
                                << liveness_score << "," << verify_core << "," << quality;
                    ssqualitypath.push_back(ssquality.str());
                    ssquality.str(""); 
                }
            }
            st_face_module_release_feature(feature, 0);
            feature = nullptr;
            
        }

        WriteFeatureFile(feature_Dir, REALFEATURE, liveFileFeature);
        liveFileFeature.clear();

        WriteStrings(livehackness, HACK_FRR_PATH);
        livehackness.clear();

        WriteStrings(ssqualitypath, REAL_QUA_DETECT_PATH);
        ssqualitypath.clear();
       
        // 遍历hack图
        for(int i = 0; i < hack_paths.size()/4; i++)
        {
            pathLeft = single_dir + "/continue/hack/" + LEFTIMAGE + std::to_string(i) + FILETYPE;
            pathRight = single_dir + "/continue/hack/" + RIGHTIMAGE + std::to_string(i) + FILETYPE;
            ret = GetPathFeature(pathLeft, pathRight, &feature, &enrollLen, &ImageRect, &liveness_score, &status, &quality);
            if(ret != 0 || status != 0 )
            {
                LOGD("getHackPathInfoImage error..ret = %d, status = %d\n", ret, status);
            }else
            {
                // 存储live图的feature
                ssfeature << pathLeft << " " << feature;
                liveFileFeature.push_back(ssfeature.str());
                ssfeature.str("");

                // 存储live图的hackness
                ssrealhackness << pathLeft << "," << liveness_score;
                livehackness.push_back(ssrealhackness.str());
                ssrealhackness.str("");

                if(compare_feature(enroll_data_, feature, &verify_core) == ST_OK )
                {
                    ssquality << pathLeft << "," << ImageRect.top << "," << ImageRect.bottom << "," << ImageRect.left << "," << ImageRect.right << ","
                                << liveness_score << "," << verify_core << "," << quality;
                    ssqualitypath.push_back(ssquality.str());
                    ssquality.str(""); 
                }
            }

            st_face_module_release_feature(feature, 0);
            feature = nullptr;
        }

        WriteFeatureFile(feature_Dir, HACKFEATURE, liveFileFeature);
        liveFileFeature.clear();

        WriteStrings(livehackness, HACK_FAR_PATH);
        livehackness.clear();

        WriteStrings(ssqualitypath, HACK_QUA_DETECT_PATH);
        ssqualitypath.clear();

        
        //  释放enroll图的feature
        for(int i = 0; i < enroll_data_.size(); i++)
        {
            // LOGD("delete feature[%d] address %p\n", i, enroll_data_[i].c_str());
            st_face_module_release_feature((char *)(enroll_data_[i].c_str()), 0);
            // LOGD("delete feature\n");
        }
        enroll_data_.clear();
    }


    return ST_OK;
}

st_result_t STLockEvaluation::compare_feature(std::vector<std::string>&enroll, const char* feature, float *score)
{
     float verifyScore = 0;
    *score = verifyScore;

    st_result_t ret = 0;

    for(auto &str : enroll)
    {
        if(st_face_module_compare_feature(str.c_str(), feature, &verifyScore) == ST_OK)
        {
            if (verifyScore > *score) {
                *score = verifyScore;
            }
            
        }else
        {
            *score = 0;
            LOGD("compare_feature in the function compare_feature\n");
            return -1;
        }
    }
    
    return ST_OK;
}

st_result_t STLockEvaluation::GetPathFeature(std::string pathleft, std::string pathright, char **feature, int *featureLen, st_rect_t *rect, float *liveness_score, int *status, float *quality)
{
    int ret(ST_OK);
    
    char *contentleft = nullptr;
    char *contentright = nullptr;
    std::unique_ptr< char[] > content_ptrleft;
    std::unique_ptr< char[] > content_ptrright;
    st_module_image_t cur_img;

    int i = 0;

    int fileLen;
    ret = readFile(pathleft.c_str(), &contentleft, fileLen);    
    if (ret != ST_OK) {
        LOGD("read file error and ret = %d\n", ret);
        return ret;
    } 
    content_ptrleft.reset(contentleft);

    ret = readFile(pathright.c_str(), &contentright, fileLen);
    if (ret != ST_OK) {
        return ret;
    }
    content_ptrright.reset(contentright);


    cur_img.first_image = ( unsigned char * )contentleft;
    cur_img.second_image = ( unsigned char *)contentright;
    cur_img.pixel_format = ST_PIX_FMT_GRAY8;
    cur_img.orientation = ST_FACE_LEFT;
    cur_img.stride       = 640;
    cur_img.height       = 400;
    cur_img.width        = 640;

  
    return st_face_module_get_image_info(&cur_img, feature, featureLen, rect, liveness_score, quality, status);
}


st_result_t STLockEvaluation::WriteFeatureFile(std::string dir, std::string filename, const std::vector<std::string> &feature)
{
    if (!createDir(dir.c_str()))
    {    
        LOGD("create Dir fail in the function WriteFeatureFile\n");
        return -1;
    }
   
    std::string file_path = dir + "/" + filename;

    std::fstream infeaturefile(file_path, std::ios::out);
    if(!infeaturefile.is_open())
    {
        LOGD("WriteFeatureFile ERROR\n");
        return -1;
    }

    std::stringstream ss;
    for(auto &str : feature)
    {
        ss << str << std::endl;
    }

    std::string log_str = ss.str();
    infeaturefile.write(log_str.c_str(), log_str.size());

    infeaturefile.close();

    return ST_OK;
}


st_result_t STLockEvaluation::Get_Enrollfeature(std::string pathleft, std::string pathright, char **feature, int *feature_len, int *status)
{
    int ret(ST_OK);
    
    //= {( unsigned char * )content, config_.pixel_format, config_.image_width, config_.image_height, config_.image_width};
    char *contentleft = nullptr;
    char *contentright = nullptr;
    std::unique_ptr< char[] > content_ptrleft;
    std::unique_ptr< char[] > content_ptrright;
    st_module_image_t cur_img;

    int i = 0;

    int fileLen;
    ret = readFile(pathleft.c_str(), &contentleft, fileLen);    
    if (ret != ST_OK) {
        LOGD("read file error and ret = %d\n", ret);
        return ret;
    } 
    content_ptrleft.reset(contentleft);

    ret = readFile(pathright.c_str(), &contentright, fileLen);
    if (ret != ST_OK) {
        return ret;
    }
    content_ptrright.reset(contentright);


    cur_img.first_image = ( unsigned char * )contentleft;
    cur_img.second_image = ( unsigned char *)contentright;
    cur_img.pixel_format = ST_PIX_FMT_GRAY8;
    cur_img.orientation = ST_FACE_LEFT;
    cur_img.stride       = 640;
    cur_img.height       = 400;
    cur_img.width        = 640;

    return st_face_module_get_feature(&cur_img, feature, feature_len, status);
}


st_result_t STLockEvaluation::Evalution()
{
    // 初始化生成路径
    st_result_t ret;
    Init();
    // 遍历根目录下有多少人
    std::vector< std::string > allDirLists;
    bool bRet;
    bRet = getDirectoryList(evaConfig_.evalutaion_data_dir.c_str(), allDirLists);
    if (!bRet) {
        LOGD("Can't READ Evaluation pick_test directory");
        return ST_FACE_AUTH_ERROR_READ_EVALUATION_DIRECTORY;
    }
    if (allDirLists.size() == 0) {
        LOGD("No test data directory\n");
        return ST_OK;
    }
    
    ExtractFaceInfo(allDirLists);
    
    // 遍历feature目录
    allDirLists.clear();
    bRet = getDirectoryList(evaConfig_.feature_dir.c_str(), allDirLists);
    if (!bRet) {
        LOGE("Can't READ Evaluation feature directory");
        return ST_FACE_AUTH_ERROR_READ_EVALUATION_DIRECTORY;
    }

    // 计算verify值
    calVerify(allDirLists);
    // 计算最后表格
    ReportResult();
}

st_result_t STLockEvaluation::calVerify(std::vector< std::string > &allSingleDirLists)
{
    st_result_t ret;
    
    for(auto& str : allSingleDirLists)
    {
        std::string enrollpath;
        std::vector< std::string >enrollfeature;
        std::vector< std::string > fileList;
        LOGD("str is %s\n", str.c_str());
        if(true != getFileList(str.c_str(), fileList))
        {
            LOGD("@@@@@@@@@@@@@@@@@@@@ERROR\n");
        }else
        {

            for(auto &fileName : fileList)
            {
                std::string::size_type position;
                position = fileName.find(ENROLLFEATURE);
                // frr
                if( position != std::string::npos)
                {
                    readFeatureFile(fileName, enrollpath, enrollfeature);
                    // LOGD("\n\nfileName is %s\n\n and the enrollpath is %s\n\n\n", fileName.c_str(), enrollpath.c_str());
                    break;
                }
            }

            // 计算本人real图verify(FRR)
            for(auto &fileName : fileList)
            {
                std::string::size_type position;
                position = fileName.find(REALFEATURE);
                
                if(position != std::string::npos)
                {
                    ret = CalSingleVerify(enrollpath, enrollfeature, fileName, VERIFY_FRR_PATH);
                    break;
                }
            }

            // 计算其他人real图verify(FAR)
            ret = CalOthersVeriy(enrollpath, enrollfeature, str, allSingleDirLists);

        }

        enrollfeature.clear();
        enrollpath.clear();
    }


    return ret;
}

st_result_t STLockEvaluation::CalOthersVeriy(std::string &enrollpath, std::vector<std::string>& enrollfeature, std::string currentDir, std::vector<std::string>& allSingleDirLists)
{
    st_result_t ret;
    for(auto& str : allSingleDirLists)
    {
        std::vector< std::string > fileList;
        if (str != currentDir) 
        {
            if(true != getFileList(str.c_str(), fileList))
            {
                LOGD("@@@@@@@@@@@@@@@@@@@@ERROR\n");
            }else
            {
                for(auto &fileName : fileList)
                {
                    std::string::size_type position;
                    position = fileName.find(REALFEATURE);
                    if (position != std::string::npos) 
                    {
                        CalSingleVerify(enrollpath, enrollfeature, fileName, VERIFY_FAR_PATH);
                    }
                    
                }
            }
        }
        
    }
}

st_result_t STLockEvaluation::CalSingleVerify(std::string &enrollpath, std::vector<std::string>&enrollfeature, std::string& realFeaturename, const std::string& dtsfile)
{
    std::fstream inFeatureFile(realFeaturename, std::ios::in);
    if( !inFeatureFile )
    {
        LOGD("File[%s] could not be opened\n", realFeaturename.c_str()); 
    }

    std::string fea;
    std::string path;
    float score = 0;

    std::stringstream ss;
    std::vector<std::string>frrString;
    while(inFeatureFile >> path >> fea)
    {
       compare_feature(enrollfeature, fea.c_str(), &score);
       ss << enrollpath << "," << path << "," << score;
       score = 0;
       frrString.push_back(ss.str());
       ss.str("");
    }
    inFeatureFile.close();

    WriteStrings(frrString, dtsfile);

    return ST_OK;
}

 st_result_t STLockEvaluation::readhackfile(std::string& hackfile, std::vector<std::string>& hackness)
 {
     std::fstream inFeatureFile(hackfile, std::ios::in);
    if( !inFeatureFile )
    {
       // LOGD("File[%s] could not be opened\n", otherRealName.c_str()); 
    }
    std::string line;
    // 空出标题行
    std::getline(inFeatureFile, line);
    while(std::getline(inFeatureFile, line))
    {
        std::istringstream sin(line);
        std::vector<std::string> fields;
        std::string field;
        while(std::getline(sin, field, ','))
        {
            fields.push_back(field);
        }
        hackness.push_back(fields[1]);
        fields.clear();
        line.clear();
    }
    
    return ST_OK;
 }

 st_result_t STLockEvaluation::readverifyfile(std::string& verifyfile, std::vector<std::string> &verify)
 {
     std::fstream inFeatureFile(verifyfile, std::ios::in);
    if( !inFeatureFile )
    {
       // LOGD("File[%s] could not be opened\n", otherRealName.c_str()); 
    }
    std::string line;
    // 空出标题行
    std::getline(inFeatureFile, line);
    while(std::getline(inFeatureFile, line))
    {
        std::istringstream sin(line);
        std::vector<std::string> fields;
        std::string field;
        while(std::getline(sin, field, ','))
        {
            fields.push_back(field);
        }
        verify.push_back(fields[2]);
        fields.clear();
        line.clear();
    }
    return ST_OK;
 }

st_result_t STLockEvaluation::readFeatureFile(std::string fileName, std::string &enrollpath, std::vector<std::string>&enrollfeature)
{
    std::fstream inFeatureFile(fileName, std::ios::in);
    if( !inFeatureFile )
    {
        LOGD("File[%s] could not be opened\n", fileName.c_str()); 
    }

    std::string fea;
    std::string path;
    while(inFeatureFile >> path >> fea){
        enrollfeature.push_back(fea);
    }
    enrollpath = path;

    inFeatureFile.close();

    return ST_OK;
}

st_result_t STLockEvaluation::CheckConfig()
{
    if (evaConfig_.evalutaion_data_dir.empty() || evaConfig_.image_height <= 0 || evaConfig_.image_width <= 0) {
        return ST_E_INVALIDARG;
    }
    
    return ST_OK;
}

bool STLockEvaluation::getDirectoryList(const char *rootPath, std::vector< std::string > &dirList)
{
     if (!rootPath) {
        return false;
    }
    DIR *dir = opendir(rootPath);
    if (!dir) {
        return false;
    }
    std::vector< std::string > dirListTmp;
    struct dirent *entry = readdir(dir);
    while (entry != NULL) {
        if (entry->d_type == DT_DIR) {
            std::string dir_name = entry->d_name;
            if (dir_name != "." && dir_name != ".." && dir_name != "model") {
               //  LOGD("dir_name is %s\n", dir_name.c_str());
                dirListTmp.push_back(std::string(rootPath) + "/" + dir_name);
            }
        }
        entry = readdir(dir);
    }
    closedir(dir);
    dirListTmp.swap(dirList);
    return true;
}

bool STLockEvaluation::getFileList(const char *basePath, std::vector< std::string > &fileList)
{
    bool ret = true;

    static const int MAXBUFSIZE = 1024;

    char buf[1024] = {0};
    getcwd(buf, MAXBUFSIZE);

    DIR *dir;
    struct dirent *ptr;
    char base[1000] = {0};
    if ((dir = opendir(basePath)) == nullptr) {
        return false;
    }

    while ((ptr = readdir(dir)) != nullptr) {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)  /// current dir OR parrent dir
            continue;
        else if (ptr->d_type == 8) {  
            std::string strImagePath = std::string(basePath) + "/" + ptr->d_name;
           
                fileList.push_back(std::move(strImagePath));
                
           
        } else if (ptr->d_type == 10) {  /// link file
                                         // printf("link file:%s/%s/%s\n", buf, basePath, ptr->d_name);
        } else if (ptr->d_type == 4) {   /// dir
            memset(base, '\0', sizeof(base));
            strcpy(base, basePath);
            strcat(base, "/");
            strcat(base, ptr->d_name);
            getFileList(base, fileList);
        }
    }
    closedir(dir);
    return true;


    return ret;
}

bool STLockEvaluation::IsFileExist(const char *path)
{
    if (!path) return false;
    if (access(path, F_OK) == -1)
    {
        // 不存在
        return false;
    }
    return true;
}

bool STLockEvaluation::createDir(const char *sPathName)
{
    char DirName[256];
    strcpy(DirName, sPathName);
    int i, len = strlen(DirName);
    if (DirName[len - 1] != '/') strcat(DirName, "/");
    len = strlen(DirName);
    for (i = 1; i < len; i++) {
        if (DirName[i] == '/') {
            DirName[i] = 0;
            if (access(DirName, 0) != 0) {
                if (mkdir(DirName, 0755) == -1) {
                    LOGE("mkdir %s error", sPathName);
                    return false;
                }
            }
            DirName[i] = '/';
        }
    }
    return true;
}

st_result_t STLockEvaluation::readFile(const char *filePath, char **content, int &nFileLen)
{
    FILE *pF = NULL;
    pF = fopen(filePath, "r");
    if (pF == NULL) {
        return -1;
    }
    fseek(pF, 0, SEEK_END);
    nFileLen = ftell(pF);
    rewind(pF);
    char *szBuffer = new (std::nothrow) char[nFileLen + 1];
    if (!szBuffer) {
        fclose(pF);
        return ST_E_OUTOFMEMORY;
    }
    nFileLen = fread(szBuffer, sizeof(char), nFileLen, pF);
    szBuffer[nFileLen] = '\0';
    fclose(pF);
    *content = szBuffer;
    return ST_OK;
}


st_result_t STLockEvaluation::Init()
{
    LOGD("init begin\n");
    st_result_t ret;
    ret = InitSdk();
    if (ret != ST_OK) 
    {
        LOGD("InitSdk failed and ERROR num is %d\n", ret);
        return ret;
    }
    
    ret = InitResultDir();
    if (ret != ST_OK) 
    {
        LOGD("InitResultDir failed and ERROR num is %d\n", ret);
        return ret;
    }
    LOGD("init end");
}


int main_()
{
    std::shared_ptr< STLockEvaluation > pointer = (std::shared_ptr< STLockEvaluation >) new STLockEvaluation(Config);

    pointer->Evalution();

    return 0;
}
