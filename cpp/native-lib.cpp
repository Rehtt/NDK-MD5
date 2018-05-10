#include <jni.h>
#include <string>
#include <malloc.h>
#include "MD5.c"


char* jstringToChar(JNIEnv* env, jstring jstr) {
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char*) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}

jstring charTojstring(JNIEnv* env, const char* pat) {
    //定义java String类 strClass
    jclass strClass = (env)->FindClass("Ljava/lang/String;");
    //获取String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
    jmethodID ctorID = (env)->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
    //建立byte数组
    jbyteArray bytes = (env)->NewByteArray(strlen(pat));
    //将char* 转换为byte数组
    (env)->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    // 设置String, 保存语言类型,用于byte数组转换至String时的参数
    jstring encoding = (env)->NewStringUTF("GB2312");
    //将byte数组转换为java String,并输出
    return (jstring) (env)->NewObject(strClass, ctorID, bytes, encoding);
}

extern "C" JNIEXPORT jstring
JNICALL

Java_com_rehtt_test_a3_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */,
    jstring jstring1) {


    char* str = (char *)env->GetStringUTFChars(jstring1,0);
    int leng = env->GetStringLength(jstring1);
    unsigned char encrypt[leng];
    for (int i = 0; i < leng; ++i) {
        encrypt[i]=str[i];
    }
    unsigned char decrypt[16];

    MD5_CTX md5;
    MD5Init(&md5);
    MD5Update(&md5,encrypt,strlen((char*)encrypt));
    MD5Final(&md5,decrypt);

    for (int i = 0; i < 16; ++i) {
        printf("%02x",decrypt[i]);
    }
    char a[32]={0};
    for (int i = 0; i < 16; ++i) {
        sprintf(a,"%s%02x",a,decrypt[i]);
    }

    return env->NewStringUTF(a);
}

