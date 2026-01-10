include("D:/QT projekti/BlurtNote2/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/BlurtNote2-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "D:/QT projekti/BlurtNote2/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/BlurtNote2.exe"
    GENERATE_QT_CONF
)
