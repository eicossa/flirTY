#ifndef EXEC_H
#define EXEC_H
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>

#include <QStringList>

std::string exec(const std::string cmd);
std::string windows_exec(const std::string cmd);
void windowsexec6();

void exiftoolExtractThermalWindows(QStringList qstrlist);
void convertBinary2GrayscaleWindows(QStringList qstrlist);

double exiftoolExtractTagWindows(const std::string tagname);
#endif // EXEC_H
