#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{   
    setWindowTitle("");
    QScreen *screen = QGuiApplication::primaryScreen();

    // Получение размеров экрана
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    // Установка размера главного окна в 60% от размеров экрана
    int mainWindowWidth = 0.6 * screenWidth;
    int mainWindowHeight = 0.6 * screenHeight;

    resize(mainWindowWidth, mainWindowHeight);

    int x = (screenWidth - this->width()) / 2;
    int y = (screenHeight - this->height()) / 2;

    move(x, y); 

    image = new QLabel(this);
    pixmap = new QPixmap();

    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100); // Установка диапазона
    progressBar->setValue(0); // Установка начального значения
    progressBar->setVisible(false); // Скрытие индикатора прогресса


    image->setMaximumWidth(screenWidth*0.8);
    image->setMaximumHeight(screenHeight*0.8);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *btnlayout = new QHBoxLayout(this);
    QHBoxLayout *imagelayout = new QHBoxLayout(this);

    btnlayout->addWidget(progressBar);

    imagelayout->addWidget(image);
    imagelayout->setAlignment(Qt::AlignCenter);
    mainlayout->addLayout(imagelayout);
    mainlayout->addLayout(btnlayout);

    QMenuBar *menuBar = new QMenuBar(this);
    QMenu *fileMenuFile = menuBar->addMenu(tr("&File"));
    QAction *openAction = fileMenuFile->addAction(tr("&Open"));
    QAction *saveAction = fileMenuFile->addAction(tr("&Save"));

    QMenu *fileMenuFilter = menuBar->addMenu(tr("&Filter"));
    QAction *colortogreyAction = fileMenuFilter->addAction(tr("&Color to Grey"));
    QAction *inverseAction = fileMenuFilter->addAction(tr("&Inverse"));
    QAction *greyworldAction = fileMenuFilter->addAction(tr("&GreyWorld"));
    QAction *LinearCorrectionAction = fileMenuFilter->addAction(tr("&LinearCorrection"));
    QAction *GaussianFilterAction = fileMenuFilter->addAction(tr("&GaussianFilter"));
    QAction *ShapeFilterAction = fileMenuFilter->addAction(tr("&ShapeFilter"));
    QAction *TrenFilterAction = fileMenuFilter->addAction(tr("&TrenFilter"));
    QAction *MedianFilterAction = fileMenuFilter->addAction(tr("&MedianFilter"));
    QAction *DilationAction = fileMenuFilter->addAction(tr("&DilationFilter"));
    QAction *TopHatAction = fileMenuFilter->addAction(tr("&TopHatFilter"));
    QAction *PancilDrowAction = fileMenuFilter->addAction(tr("&PancilDrowFilter"));
    QAction *DiagAction = fileMenuFilter->addAction(tr("&DiagFilter"));


    connect(inverseAction, &QAction::triggered, this, &Widget::Inverse);
    connect(colortogreyAction, &QAction::triggered, this, &Widget::ColorToGrey);
    connect(greyworldAction, &QAction::triggered, this, &Widget::GreyWorld);
    connect(LinearCorrectionAction, &QAction::triggered, this, &Widget::LinearCorrection);
    connect(GaussianFilterAction, &QAction::triggered, this, &Widget::GaussianFilter);
    connect(ShapeFilterAction, &QAction::triggered, this, &Widget::ShapeFilter);
    connect(TrenFilterAction, &QAction::triggered, this, &Widget::TrenFilter);
    connect(MedianFilterAction, &QAction::triggered, this, &Widget::MedianFilter);
    connect(DilationAction, &QAction::triggered, this, &Widget::Dilation);
    connect(TopHatAction, &QAction::triggered, this, &Widget::TopHat);
    connect(PancilDrowAction, &QAction::triggered, this, &Widget::PancilDrow);
    connect(DiagAction, &QAction::triggered, this, &Widget::Diag);
    connect(openAction, &QAction::triggered, this, &Widget::OpenFile);
    connect(saveAction, &QAction::triggered, this, &Widget::Save);
    mainlayout->setMenuBar(menuBar);
}

Widget::~Widget()
{
    delete layout()->menuBar();
}

void Widget::ColorToGrey() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    // Преобразуем цветное изображение в черно-белое
    QImage imageConverted = pixmap->toImage();
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y/imageConverted.height())*100));
        for (int x = 0; x < imageConverted.width(); ++x) {
            QRgb color = imageConverted.pixel(x, y);
            int gray = qGray(color);
            imageConverted.setPixel(x, y, qRgb(gray, gray, gray));
        }
    }
    // Отображаем преобразованное изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = QPixmap::fromImage(imageConverted);
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}

void Widget::Inverse() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    // Преобразуем изображение в формате QImage
    QImage imageConverted = pixmap->toImage();

    // Инвертируем каждый пиксель
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y/imageConverted.height())*100));
        for (int x = 0; x < imageConverted.width(); ++x) {
            QRgb color = imageConverted.pixel(x, y);
            int red = 255 - qRed(color); // Инвертируем компоненту красного цвета
            int green = 255 - qGreen(color); // Инвертируем компоненту зеленого цвета
            int blue = 255 - qBlue(color); // Инвертируем компоненту синего цвета
            imageConverted.setPixel(x, y, qRgb(red, green, blue)); // Устанавливаем новый цвет пикселя
        }
    }

    // Отображаем преобразованное изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = QPixmap::fromImage(imageConverted); // Обновляем pixmap с инвертированным изображением
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio); // Масштабируем изображение
    image->setPixmap(*pixmap); // Устанавливаем pixmap в качестве изображения для QLabel
    progressBar->setVisible(false);
}

void Widget::GreyWorld() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    // Преобразуем изображение в формате QImage
    QImage imageConverted = pixmap->toImage();

    // Инвертируем каждый пиксель
    int N = 0;
    double SumR = 0;
    double SumG = 0;
    double SumB = 0;
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y/imageConverted.height())*50));
        for (int x = 0; x < imageConverted.width(); ++x) {
            QRgb color = imageConverted.pixel(x, y);
            SumR += qRed(color); // Инвертируем компоненту красного цвета
            SumG += qGreen(color); // Инвертируем компоненту зеленого цвета
            SumB += qBlue(color); // Инвертируем компоненту синего цвета
            //imageConverted.setPixel(x, y, qRgb(red, green, blue)); // Устанавливаем новый цвет пикселя
            N++;
        }
    }
    std::cout<<"strart";
    SumR = SumR / N;
    SumG = SumG / N;
    SumB = SumB / N;
    double Avg = (SumB+SumG+SumR)/3;
    std::cout<<"end";
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue(50+(int)(((float)y/imageConverted.height())*50));
        for (int x = 0; x < imageConverted.width(); ++x) {
            QRgb color = imageConverted.pixel(x, y);
            double red = qRed(color)*Avg/SumR; // Инвертируем компоненту красного цвета
            double green = qGreen(color)*Avg/SumG; // Инвертируем компоненту зеленого цвета
            double blue = qBlue(color)*Avg/SumB; // Инвертируем компоненту синего цвета
            imageConverted.setPixel(x, y, qRgb((int)red, (int)green, (int)blue)); // Устанавливаем новый цвет пикселя
        }
    }
    // Отображаем преобразованное изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = QPixmap::fromImage(imageConverted); // Обновляем pixmap с инвертированным изображением
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio); // Масштабируем изображение
    image->setPixmap(*pixmap); // Устанавливаем pixmap в качестве изображения для QLabel
    progressBar->setVisible(false);
}

void Widget::OpenFile() {
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Выберите изображение", "", "Изображения (*.png *.jpg *.bmp)");
    if (!fileName.isEmpty()) {
        delete pixmap;
        pixmap = new QPixmap(fileName);
        if (!pixmap->isNull()) {
            QScreen *screen = QGuiApplication::primaryScreen();
            QRect screenGeometry = screen->geometry();
            int screenWidth = screenGeometry.width();
            int screenHeight = screenGeometry.height();
            image->setPixmap(pixmap->scaled(screenWidth*0.7 , screenHeight*0.7, Qt::KeepAspectRatio));
            //image->setPixmap(*pixmap);
        } else {
            QMessageBox::critical(this,"Ошибка", "Ошибка загрузки изображения");
        }
    }
}

void Widget::Save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), "", tr("Images (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) { // Проверяем, был ли выбран файл для сохранения
    pixmap; // Ваш объект QPixmap с изображением

    // Сохранение изображения в выбранный файл
    if (!pixmap->isNull()) { // Проверка наличия изображения в объекте pixmap
        if (pixmap->save(fileName)) { // Сохранение изображения в файл
        } else {
            QMessageBox::critical(this,"Ошибка", "Ошибка при сихранении");
        }
    } else {
        QMessageBox::critical(this,"Ошибка", "Изображения нет.");
    }
    }
}

void Widget::LinearCorrection() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    // Преобразуем изображение в формате QImage
    QImage imageConverted = pixmap->toImage();

    // Инвертируем каждый пиксель
    int RMax = 0;
    int GMax = 0;
    int BMax = 0;
    int RMin = 255;
    int GMin = 255;
    int BMin = 255;
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y/imageConverted.height())*50));
        for (int x = 0; x < imageConverted.width(); ++x) {
            QRgb color = imageConverted.pixel(x, y);
            RMax = std::max(RMax,qRed(color));
            GMax = std::max(GMax,qGreen(color)); 
            BMax = std::max(BMax,qBlue(color));
            RMin = std::min(RMin,qRed(color));
            GMin = std::min(GMin,qGreen(color)); 
            BMin = std::min(BMin,qBlue(color));
            //imageConverted.setPixel(x, y, qRgb(red, green, blue)); // Устанавливаем новый цвет пикселя
        }
    }
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue(50+(int)(((float)y/imageConverted.height())*50));
        for (int x = 0; x < imageConverted.width(); ++x) {
            QRgb color = imageConverted.pixel(x, y);
            int red = (qRed(color)-RMin)*(255/(RMax-RMin)); // Инвертируем компоненту красного цвета
            int green = (qGreen(color)-GMin)*(255/(GMax-GMin)); // Инвертируем компоненту зеленого цвета
            int blue = (qBlue(color)-BMin)*(255/(BMax-BMin)); // Инвертируем компоненту синего цвета
            imageConverted.setPixel(x, y, qRgb(red, green, blue)); // Устанавливаем новый цвет пикселя
        }
    }
    // Отображаем преобразованное изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = QPixmap::fromImage(imageConverted); // Обновляем pixmap с инвертированным изображением
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio); // Масштабируем изображение
    image->setPixmap(*pixmap); // Устанавливаем pixmap в качестве изображения для QLabel
    progressBar->setVisible(false);
}

void Widget::GaussianFilter() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    
    // Преобразуем изображение в формат QImage
    QImage imageConverted = pixmap->toImage();
    
    // Параметры фильтра Гаусса
    int radius = 3; // Радиус фильтра
    double sigma = 1.0; // Стандартное отклонение

    // Применяем фильтр Гаусса к каждому пикселю изображения
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y / imageConverted.height()) * 100));
        for (int x = 0; x < imageConverted.width(); ++x) {
            double sumRed = 0.0, sumGreen = 0.0, sumBlue = 0.0, sumWeight = 0.0;

            // Проходим по окрестности пикселя с учетом радиуса фильтра
            for (int i = -radius; i <= radius; ++i) {
                for (int j = -radius; j <= radius; ++j) {
                    int newX = qBound(0, x + i, imageConverted.width() - 1);
                    int newY = qBound(0, y + j, imageConverted.height() - 1);
                    QRgb pixel = imageConverted.pixel(newX, newY);

                    // Вычисляем вес пикселя по ядру фильтра Гаусса
                    double weight = exp(-(i * i + j * j) / (2 * sigma * sigma));

                    // Накапливаем суммы значений пикселей с учетом весов
                    sumRed += qRed(pixel) * weight;
                    sumGreen += qGreen(pixel) * weight;
                    sumBlue += qBlue(pixel) * weight;
                    sumWeight += weight;
                }
            }

            // Нормализуем значения пикселя и устанавливаем их в результирующее изображение
            imageConverted.setPixel(x, y, qRgb(static_cast<int>(sumRed / sumWeight), 
                                            static_cast<int>(sumGreen / sumWeight), 
                                            static_cast<int>(sumBlue / sumWeight)));
        }
    }

    // Обновляем pixmap с преобразованным изображением
    *pixmap = QPixmap::fromImage(imageConverted);
    
    // Масштабируем изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);

    // Отображаем преобразованное изображение в QLabel
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}

void Widget::ShapeFilter() {
// Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    
    // Преобразуем изображение в формат QImage
    QImage imageConverted = pixmap->toImage();
    
    QImage resultImage(imageConverted.size(), QImage::Format_RGB32);

    // Применяем матричный фильтр к каждому пикселю изображения
    for (int y = 1; y < imageConverted.height() - 1; ++y) {
        progressBar->setValue((int)(((float)y / (imageConverted.height() - 1)) * 100));
        for (int x = 1; x < imageConverted.width() - 1; ++x) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;

            // Проходим по окрестности пикселя с учетом размера ядра
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    QRgb pixel = imageConverted.pixel(x + i, y + j);

                    // Умножаем компоненты цвета пикселя на соответствующие значения ядра
                    sumRed += qRed(pixel) * (-1);
                    sumGreen += qGreen(pixel) * (-1);
                    sumBlue += qBlue(pixel) * (-1);
                }
            }

            // Умножаем компоненты цвета пикселя по центру ядра на значение ядра
            QRgb centerPixel = imageConverted.pixel(x, y);
            sumRed += qRed(centerPixel) * 10;
            sumGreen += qGreen(centerPixel) * 10;
            sumBlue += qBlue(centerPixel) * 10;

            // Ограничиваем значения компонентов до диапазона [0, 255]
            sumRed = qBound(0, sumRed, 255);
            sumGreen = qBound(0, sumGreen, 255);
            sumBlue = qBound(0, sumBlue, 255);

            // Устанавливаем новые компоненты цвета в результирующее изображение
            resultImage.setPixel(x, y, qRgb(sumRed, sumGreen, sumBlue));
        }
    }

    // Обновляем pixmap с преобразованным изображением
    *pixmap = QPixmap::fromImage(resultImage);
    
    // Масштабируем изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);

    // Отображаем преобразованное изображение в QLabel
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}

void Widget::TrenFilter() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    
    // Преобразуем изображение в формат QImage
    QImage imageConverted = pixmap->toImage();
    
    // Создаем пустое изображение для результата
    QImage resultImage(imageConverted.size(), QImage::Format_RGB32);

    // Задаем ядро преобразования
    int kernel[3][3] = {{0, 1, 0},
                        {1, 0, -1},
                        {0, -1, 0}};

    // Применяем матричный фильтр к каждому пикселю изображения
    for (int y = 1; y < imageConverted.height() - 1; ++y) {
        progressBar->setValue((int)(((float)y / (imageConverted.height() - 1)) * 100));
        for (int x = 1; x < imageConverted.width() - 1; ++x) {
            int sumRed = 0, sumGreen = 0, sumBlue = 0;

            // Проходим по окрестности пикселя с учетом размера ядра
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    QRgb pixel = imageConverted.pixel(x + i, y + j);

                    // Умножаем компоненты цвета пикселя на соответствующие значения ядра
                    sumRed += qRed(pixel) * kernel[i + 1][j + 1];
                    sumGreen += qGreen(pixel) * kernel[i + 1][j + 1];
                    sumBlue += qBlue(pixel) * kernel[i + 1][j + 1];
                }
            }

            // Ограничиваем значения компонентов до диапазона [0, 255]
            sumRed = qBound(0, sumRed, 255);
            sumGreen = qBound(0, sumGreen, 255);
            sumBlue = qBound(0, sumBlue, 255);

            // Устанавливаем новые компоненты цвета в результирующее изображение
            resultImage.setPixel(x, y, qRgb(sumRed, sumGreen, sumBlue));
        }
    }

    // Обновляем pixmap с преобразованным изображением
    *pixmap = QPixmap::fromImage(resultImage);
    
    // Масштабируем изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);

    // Отображаем преобразованное изображение в QLabel
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}
void Widget::MedianFilter() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    
    // Преобразуем изображение в формат QImage
    QImage imageConverted = pixmap->toImage();
    
    // Создаем пустое изображение для результата
    QImage resultImage(imageConverted.size(), QImage::Format_RGB32);

    // Применяем медианный фильтр к каждому пикселю изображения
    for (int y = 1; y < imageConverted.height() - 1; ++y) {
        progressBar->setValue((int)(((float)y / (imageConverted.height() - 1)) * 100));
        for (int x = 1; x < imageConverted.width() - 1; ++x) {
            QVector<int> redValues, greenValues, blueValues;

            // Проходим по окрестности пикселя с учетом размера окна фильтра
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    QRgb pixel = imageConverted.pixel(x + i, y + j);

                    // Собираем значения компонент цвета в окрестности пикселя
                    redValues.append(qRed(pixel));
                    greenValues.append(qGreen(pixel));
                    blueValues.append(qBlue(pixel));
                }
            }

            // Сортируем значения компонент цвета для каждого канала
            std::sort(redValues.begin(), redValues.end());
            std::sort(greenValues.begin(), greenValues.end());
            std::sort(blueValues.begin(), blueValues.end());

            // Выбираем медианные значения для каждого канала
            int medianRed = redValues.at(redValues.size() / 2);
            int medianGreen = greenValues.at(greenValues.size() / 2);
            int medianBlue = blueValues.at(blueValues.size() / 2);

            // Устанавливаем медианные значения компонент цвета в результирующее изображение
            resultImage.setPixel(x, y, qRgb(medianRed, medianGreen, medianBlue));
        }
    }

    // Обновляем pixmap с преобразованным изображением
    *pixmap = QPixmap::fromImage(resultImage);
    
    // Масштабируем изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);

    // Отображаем преобразованное изображение в QLabel
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}

void Widget::Dilation() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    
    // Преобразуем изображение в формат QImage
    QImage imageConverted = pixmap->toImage();
    
    // Создаем пустое изображение для результата
    QImage resultImage(imageConverted.size(), QImage::Format_RGB32);

    // Определяем структурирующий элемент (ядро) для операции расширения
    const int kernelSize = 3;
    int kernel[kernelSize][kernelSize] = {
        {-1, -1, -1},
        {-1,  9, -1},
        {-1, -1, -1}
    };

    // Применяем операцию расширения к каждому пикселю изображения
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y / imageConverted.height()) * 100));
        for (int x = 0; x < imageConverted.width(); ++x) {
            int maxRed = 0, maxGreen = 0, maxBlue = 0;

            // Проходим по окрестности пикселя с учетом размера ядра
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    // Получаем координаты пикселя в окрестности
                    int newX = qBound(0, x + j, imageConverted.width() - 1);
                    int newY = qBound(0, y + i, imageConverted.height() - 1);

                    // Получаем цвет текущего пикселя
                    QRgb pixel = imageConverted.pixel(newX, newY);

                    // Применяем ядро к текущему пикселю
                    maxRed = qMax(maxRed, qRed(pixel) + kernel[i + 1][j + 1]);
                    maxGreen = qMax(maxGreen, qGreen(pixel) + kernel[i + 1][j + 1]);
                    maxBlue = qMax(maxBlue, qBlue(pixel) + kernel[i + 1][j + 1]);
                }
            }

            // Обновляем цвет текущего пикселя в результирующем изображении
            resultImage.setPixel(x, y, qRgb(qBound(0, maxRed, 255), 
                                            qBound(0, maxGreen, 255), 
                                            qBound(0, maxBlue, 255)));
        }
    }

    // Обновляем pixmap с преобразованным изображением
    *pixmap = QPixmap::fromImage(resultImage);
    
    // Масштабируем изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);

    // Отображаем преобразованное изображение в QLabel
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}

void Widget::TopHat() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    
    // Преобразуем изображение в формат QImage
    QImage imageConverted = pixmap->toImage();
    
    // Создаем пустое изображение для результата
    QImage resultImage(imageConverted.size(), QImage::Format_RGB32);

    // Определяем структурирующий элемент (ядро) для операции Top Hat
    const int kernelSize = 3;
    int kernel[kernelSize][kernelSize] = {
        {0, 1, 0},
        {1, 0, -1},
        {0, -1, 0}
    };

    // Применяем операцию Top Hat к каждому пикселю изображения
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y / imageConverted.height()) * 100));
        for (int x = 0; x < imageConverted.width(); ++x) {
            int maxRed = 0, maxGreen = 0, maxBlue = 0;
            int minRed = 255, minGreen = 255, minBlue = 255;

            // Проходим по окрестности пикселя с учетом размера ядра
            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    // Получаем координаты пикселя в окрестности
                    int newX = qBound(0, x + j, imageConverted.width() - 1);
                    int newY = qBound(0, y + i, imageConverted.height() - 1);

                    // Получаем цвет текущего пикселя
                    QRgb pixel = imageConverted.pixel(newX, newY);

                    // Применяем ядро к текущему пикселю
                    maxRed = qMax(maxRed, qRed(pixel) + kernel[i + 1][j + 1]);
                    maxGreen = qMax(maxGreen, qGreen(pixel) + kernel[i + 1][j + 1]);
                    maxBlue = qMax(maxBlue, qBlue(pixel) + kernel[i + 1][j + 1]);
                    minRed = qMin(minRed, qRed(pixel) + kernel[i + 1][j + 1]);
                    minGreen = qMin(minGreen, qGreen(pixel) + kernel[i + 1][j + 1]);
                    minBlue = qMin(minBlue, qBlue(pixel) + kernel[i + 1][j + 1]);
                }
            }

            // Разница между максимальным и минимальным значением цветов для каждого канала
            int diffRed = maxRed - minRed;
            int diffGreen = maxGreen - minGreen;
            int diffBlue = maxBlue - minBlue;

            // Обновляем цвет текущего пикселя в результирующем изображении
            resultImage.setPixel(x, y, qRgb(qBound(0, diffRed, 255), 
                                            qBound(0, diffGreen, 255), 
                                            qBound(0, diffBlue, 255)));
        }
    }

    // Обновляем pixmap с преобразованным изображением
    *pixmap = QPixmap::fromImage(resultImage);
    
    // Масштабируем изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);

    // Отображаем преобразованное изображение в QLabel
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}

void Widget::PancilDrow() {
    this->TopHat();
    this->MedianFilter();
    this->Inverse();
    this->ColorToGrey();
}

void Widget::Diag() {
    // Проверяем, есть ли выбранное изображение
    if (pixmap->isNull()) {
        QMessageBox::information(this, "Information", "No image is selected.");
        return;
    }
    progressBar->setVisible(true);
    // Преобразуем цветное изображение в черно-белое
    QImage imageConverted = pixmap->toImage();
    for (int y = 0; y < imageConverted.height(); ++y) {
        progressBar->setValue((int)(((float)y/imageConverted.height())*100));
        for (int x = 0; x < imageConverted.width(); ++x) {
            if (-imageConverted.width()*y+x*imageConverted.height()<0) {
                QRgb color = imageConverted.pixel(imageConverted.width()-x, y);
                int gray = qGray(color);
                imageConverted.setPixel(imageConverted.width()-x, y, qRgb(gray, gray, gray));
            }
        }
    }
    // Отображаем преобразованное изображение
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();
    *pixmap = QPixmap::fromImage(imageConverted);
    *pixmap = pixmap->scaled(screenWidth * 0.7, screenHeight * 0.7, Qt::KeepAspectRatio);
    image->setPixmap(*pixmap);
    progressBar->setVisible(false);
}