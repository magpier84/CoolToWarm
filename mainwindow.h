#pragma once

#include "GridView.h"

#include <QMainWindow>

#include "ui_mainwindow.h"

#include <memory>

class ImageProcessor;

class MainWindow : public QMainWindow, protected GridViewDelegate, protected Ui::MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    QSize cellSize() const override;
    QSize gridSize() const override;
    void drawCell(QPainter* painter, const QRect& rect, int column, int row) override;

private:
    bool makeImages(const QString& fileName);
    void updateImages(qreal deviation);

private:
    std::shared_ptr<ImageProcessor> _imageProcessor;
    std::vector<QImage> _images;
    GridView* _gridView;
};
