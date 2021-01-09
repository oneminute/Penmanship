#pragma once

#include <QMainWindow>
#include <QScopedPointer>

class EditGraphicsViewer;
class EditGraphicsScene;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onActionPrint(bool checked = false);
    void onActionAddCell(bool checked = false);
    void onActionGenerate(bool checked = false);

private:
    QScopedPointer<Ui::MainWindow> m_ui;
    EditGraphicsViewer* m_viewer;
    EditGraphicsScene* m_scene;
};

