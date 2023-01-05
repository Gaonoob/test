#!/bin/bash

###################################################
# 适配步骤:
# 1. __module_fota_main 中添加芯片公司 (如已添加则不涉及)
# 2. __module_fota_xxx_main 中添加芯片型号 (如已添加则不涉及) 备注:xxx:代表mfr
# 3. __module_fota_mft_xxx 中添加FOTA包类型 (如已添加则不涉及) 备注:xxx:代表chip
# 4. __module_fota_mtr_chip_diff 中添加制作具体过程 (如已添加则不涉及)
# 5. module_comm_芯片公司_function 公共方法 后续可以独立文件维护
###################################################

module_ota_cur_path=$(cd `dirname $0`; pwd)
module_ota_old_path=${module_ota_cur_path}/module_fota_old
module_ota_new_path=${module_ota_cur_path}/module_fota_new

# 公共方法
# module_comm_芯片公司_function
__module_comm_unisoc_udx710_checkOtaName() {
    # 校验name
    unisoc_old_package_name="$(basename "$(ls -1 ${module_ota_old_path}/*.tgz)" .tgz)"
    echo unisoc_old_package_name:${unisoc_old_package_name}
    unisoc_new_package_name="$(basename "$(ls -1 ${module_ota_new_path}/*.tgz)" .tgz)"
    echo unisoc_new_package_name:${unisoc_new_package_name}

    if [[ ${unisoc_old_package_name:0:20} != ${unisoc_new_package_name:0:20} ]];then
        echo "package is Illegal"
        exit 0
    fi
}

# FOTA包命名规则
__module_comm_unisoc_udx710_makeUpOtaName() {
    unisoc_package_name=${unisoc_old_package_name:0:20}"_"${unisoc_old_package_name:21:10}"--"${unisoc_new_package_name:21:10}"_ota.swu"
    echo unisoc_up_package_name:${unisoc_package_name}
    mv ${module_ota_cur_path}/module_fota_out/up/ota.swu ${module_ota_cur_path}/module_fota_out/up/${unisoc_package_name}
}

__module_comm_unisoc_udx710_makeDownOtaName() {
    unisoc_package_name=${unisoc_old_package_name:0:20}"_("${unisoc_new_package_name:21:10}"--"${unisoc_old_package_name:21:10}")_ota.swu"
    echo unisoc_down_package_name:${unisoc_package_name}
    mv ${module_ota_cur_path}/module_fota_out/down/ota.swu ${module_ota_cur_path}/module_fota_out/down/${unisoc_package_name}
}

# 具体实现:module_fota_芯片公司_芯片型号_EX
__module_fota_unisoc_udx710_V516_diff() {
    module_ota_old_tmp_path=${module_ota_old_path}/FDDNRSEC_V516_21A
    module_ota_new_tmp_path=${module_ota_new_path}/FDDNRSEC_V516_21A

    rm -rf ${module_ota_old_tmp_path}
    rm -rf ${module_ota_new_tmp_path}
    tar xvf ${module_ota_old_path}/*.tgz -C ${module_ota_old_path}
    tar xvf ${module_ota_new_path}/*.tgz -C ${module_ota_new_path}

    # 处理bitbake 报错
    touch ${module_ota_cur_path}/layers/meta-unisoc/recipes-extended/recovery-images/unisoc-ota.bb

    # 配置环境
    MACHINE=udx710-module-m6 DISTRO=unisoc-initgc source setup-environment

    #利用 bitbake 做OTA UP包
    echo ${module_ota_old_tmp_path}
    UNISOC_BASE_DIR=${module_ota_old_tmp_path} UNISOC_TARGET_DIR=${module_ota_new_tmp_path} bitbake unisoc-ota
    cp ./tmp-unisoc_initgc-glibc/deploy/images/udx710-module-m6/unisoc-ota-udx710-module-m6.swu ${module_ota_cur_path}/module_fota_out/up/ota.swu

    # 利用 bitbake 做OTA DOWN包
    bitbake -fc cleanall unisoc-ota
    touch ${module_ota_cur_path}/layers/meta-unisoc/recipes-extended/recovery-images/unisoc-ota.bb
    UNISOC_BASE_DIR=${module_ota_new_tmp_path} UNISOC_TARGET_DIR=${module_ota_old_tmp_path} bitbake unisoc-ota
    cp ./tmp-unisoc_initgc-glibc/deploy/images/udx710-module-m6/unisoc-ota-udx710-module-m6.swu ${module_ota_cur_path}/module_fota_out/down/ota.swu

    # 重命名规则
    __module_comm_unisoc_udx710_makeUpOtaName
    __module_comm_unisoc_udx710_makeDownOtaName

    rm -rf ${module_ota_old_tmp_path}
    rm -rf ${module_ota_new_tmp_path}
}

__module_fota_unisoc_udx710_V510_diff() {
    module_ota_old_tmp_path=${module_ota_old_path}/FDDNRSEC_21A
    module_ota_new_tmp_path=${module_ota_new_path}/FDDNRSEC_21A

    rm -rf ${module_ota_old_tmp_path}
    rm -rf ${module_ota_new_tmp_path}
    tar xvf ${module_ota_old_path}/*.tgz -C ${module_ota_old_path}
    tar xvf ${module_ota_new_path}/*.tgz -C ${module_ota_new_path}

    # 处理bitbake 报错
    touch ${module_ota_cur_path}/layers/meta-unisoc/recipes-extended/recovery-images/unisoc-ota.bb

    # 配置环境
    MACHINE=udx710-module-m6 DISTRO=unisoc-initgc source setup-environment

    #利用 bitbake 做OTA UP包
    echo ${module_ota_old_tmp_path}
    UNISOC_BASE_DIR=${module_ota_old_tmp_path} UNISOC_TARGET_DIR=${module_ota_new_tmp_path} bitbake unisoc-ota
    cp ./tmp-unisoc_initgc-glibc/deploy/images/udx710-module-m6/unisoc-ota-udx710-module-m6.swu ${module_ota_cur_path}/module_fota_out/up/ota.swu

    #利用 bitbake 做OTA DOWN包
    bitbake -fc cleanall unisoc-ota
    touch ${module_ota_cur_path}/layers/meta-unisoc/recipes-extended/recovery-images/unisoc-ota.bb
    UNISOC_BASE_DIR=${module_ota_new_tmp_path} UNISOC_TARGET_DIR=${module_ota_old_tmp_path} bitbake unisoc-ota
    cp ./tmp-unisoc_initgc-glibc/deploy/images/udx710-module-m6/unisoc-ota-udx710-module-m6.swu ${module_ota_cur_path}/module_fota_out/down/ota.swu

    # 重命名规则
    __module_comm_unisoc_udx710_makeUpOtaName
    __module_comm_unisoc_udx710_makeDownOtaName

    rm -rf ${module_ota_old_tmp}
    rm -rf ${module_ota_new_tmp_path}
}

# module_fota_芯片公司_芯片型号
__module_fota_unisoc_udx710_V516() {
    echo "-------------Select Make Type-----------------"
    echo "1. make  diff img package"
    echo "2. make  full img package"
    echo "3. exit"
    read -p "please inport choice :" number

    case $((number)) in
        1) 
            __module_comm_unisoc_udx710_checkOtaName
            __module_fota_unisoc_udx710_V516_diff
            ;;
        *)
            ;;
    esac
}

__module_fota_unisoc_udx710_V510() {
    echo "-------------Select Make Type-----------------"
    echo "1. make  diff img package"
    echo "2. make  full img package"
    echo "3. exit"
    read -p "please inport choice :" number

    case $((number)) in
        1) 
            __module_comm_unisoc_udx710_checkOtaName
            __module_fota_unisoc_udx710_V510_diff
            ;;
        *)
            ;;
    esac
}

# module_fota_芯片公司_main
__module_fota_unisoc_main() {
    module_chip_array=(udx710-module udx710-module-V516 udx710-module-V510)
    echo "-------------Select chip -----------------"

    for i in "${!module_chip_array[@]}";
    do
        printf "%s\t%s\n" "$i" "${module_chip_array[$i]}"
    done

    read -p "please inport choice :" number

    case $((number)) in
        0) 
            exit
            ;;
        1)
            __module_fota_unisoc_udx710_V516
            exit 
            ;;
        2)
            __module_fota_unisoc_udx710_V510
            exit 
            ;;
        *)
            ;;
    esac
}

# main
__module_fota_main() {
    echo "-------------Select manufacturer -----------------"

    # 生成目录
    rm -rf ./module_fota_out
    mkdir -p ./module_fota_out/up
    mkdir -p ./module_fota_out/down

    # 制造商数组
    module_mfr_array=(unisoc hisi)

    for i in "${!module_mfr_array[@]}"; 
    do
        printf "%s\t%s\n" "$i" "${module_mfr_array[$i]}"
    done

    echo "other. exit"
    read -p "please inport choice :" number

    case $((number)) in
        0) 
             __module_fota_unisoc_main
            ;;
        1)
            exit 
            ;;
        *)
            rm -rf ./module_fota_out
            echo "other. exit"
            ;;
    esac
}

__module_fota_main
