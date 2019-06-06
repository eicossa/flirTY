#ifndef SHELLEXECUTOR_H
#define SHELLEXECUTOR_H

using namespace std;
#include <iostream>
#include "cmdstrings.h"
class shellExecutor
{
    std::string           imgpath;
    toolPathStrings       *tPS;
    metadataReaderStrings *mdRS;
    extractionStrings     *eS;
    conversionStrings     *cS;
    std::string           unorderedRAWs_imgpath;
    std::string           displayRAWs_imgpath;
public:
    shellExecutor(std::string, std::string, std::string);
    void readyForExtraction(int rtWidth, int rtHeight);
    double extractOneTag(std::string tagname);
    QStringList extractAllTagsInOneGo();
    bool executeRAWsExtractionCmd();
    bool executeConvertOrderedRAWsToPNG();
};


#endif // SHELLEXECUTOR_H
