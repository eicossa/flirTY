#ifndef LEAFDETECTOR_H
#define LEAFDETECTOR_H

#include <QObject>

class leafDetector : public QObject
{
    Q_OBJECT
public:
    explicit leafDetector(QObject *parent = nullptr);

    void     extractSaturation();
    void     threshold(int l);
    void     medianBlur();


    void extractBlue();

    void logicalOR();
    void applyMask();

    void rgb2gray_lab();
    void extract_a();
    void extract_b();

    void apply_mask();
    void find_objects();
    void roi_objects();
    void object_composition();
    void analyse_object();





signals:

};

#endif // LEAFDETECTOR_H
