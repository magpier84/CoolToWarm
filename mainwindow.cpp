#include "mainwindow.h"

#include "Filters/ImageProcessor.h"
#include "Filters/oRGBFilter.h"
#include "Filters/ColorShiftFilter.h"

#include <QDebug>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QLabel>
#include <QPainter>
#include <QStandardPaths>
#include <QVBoxLayout>

#include "ui_mainwindow.h"

namespace
{
constexpr int RowsCount = 3;
constexpr int ColumnsCount = 3;
constexpr qreal MinDeviation = 0.0;
constexpr qreal MaxDevaation = 0.5;
constexpr qreal DefaultDeviation = 0.15;

QImage convertToQImage(const ImageProcessor::Image& image)
{
    QImage result(image.width(), image.height(), QImage::Format_ARGB32);

    const float* srcR = image.data(0, 0, 0, 0);
    const float* srcG = image.data(0, 0, 0, 1);
    const float* srcB = image.data(0, 0, 0, 2);

    auto* dst = result.bits();

    for (auto i = 0; i < image.width() * image.height(); ++i)
    {
        dst[2] = 255 * *srcR++;
        dst[1] = 255 * *srcG++;
        dst[0] = 255 * *srcB++;
        dst[3] = 255;

        dst += 4;
    }

    return result;
}

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    auto* frame = new QWidget();
    auto* mainLayout = new QVBoxLayout(frame);

    _gridView = new GridView(this);
    _gridView->setDelegate(this);
    mainLayout->addWidget(_gridView);

    auto* toolBox = new QWidget();
    toolBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    toolBox->setEnabled(false);
    auto* toolBoxLayout = new QHBoxLayout(toolBox);
    toolBoxLayout->addStretch();
    toolBoxLayout->addWidget(new QLabel("Deviation:"));

    auto* deviationSpin = new QDoubleSpinBox();
    deviationSpin->setRange(0, 0.5);
    deviationSpin->setSingleStep(0.05);
    deviationSpin->setValue(DefaultDeviation);

    connect(deviationSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [=](qreal value)
    {
        deviationSpin->setEnabled(false);
        updateImages(value);
        deviationSpin->setEnabled(true);
    });

    toolBoxLayout->addWidget(deviationSpin);
    toolBoxLayout->addStretch();

    mainLayout->addWidget(toolBox);

    setCentralWidget(frame);

    connect(actionOpen, &QAction::triggered, [=]()
    {
        const auto fileName =
                QFileDialog::getOpenFileName(this, "Open Image",
                                             QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
                                             tr("Images (*.png)"));

        if (!fileName.isEmpty())
        {
            if (makeImages(fileName))
            {
                toolBox->setEnabled(true);
            }
        }
    });
}

QSize MainWindow::cellSize() const
{
    if (!_images.empty())
    {
        return {_images.front().width(), _images.front().height()};
    }

    return {};
}

QSize MainWindow::gridSize() const
{
    return {ColumnsCount, RowsCount};
}

void MainWindow::drawCell(QPainter* painter, const QRect& rect, int column, int row)
{
    if (_images.empty())
    {
        return;
    }

    const auto index = row * ColumnsCount + column;

    const auto image = _images[index].scaled(rect.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter->drawImage(rect, image);
}

bool MainWindow::makeImages(const QString& fileName)
{
    try
    {
        _imageProcessor = std::make_shared<ImageProcessor>(fileName.toStdString());

        _images.resize(RowsCount * ColumnsCount);

        const auto centalIndex = (RowsCount / 2) * ColumnsCount + ColumnsCount / 2;
        _images[centalIndex] = convertToQImage(_imageProcessor->getImage());

        _imageProcessor->apply(sRgb2oRgbFilter());

        updateImages(DefaultDeviation);

        return true;
    }
    catch (const std::exception& e)
    {
        qWarning() << "Exception: " << e.what();
    }

    return false;
}

void MainWindow::updateImages(qreal deviation)
{
    for (auto j = 0; j < RowsCount; ++j)
    {
        for (auto i = 0; i < ColumnsCount; ++i)
        {
            if (i != 1 || j != 1)
            {
                ImageProcessor::Image image;

                _imageProcessor->apply(ColorShiftFilter((i - 1) * deviation, (1 - j) * deviation), image);

                ImageProcessor processor(image);
                processor.apply(oRgb2sRgbFilter());

                const auto index = j * ColumnsCount + i;
                _images[index] = convertToQImage(processor.getImage());
            }
        }
    }

    _gridView->update();
}
