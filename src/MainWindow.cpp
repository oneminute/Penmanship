#include "MainWindow.h"
#include <ui/ui_MainWindow.h>
#include "EditGraphicsViewer.h"
#include "EditGraphicsScene.h"
#include "CellItem.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    m_viewer = new EditGraphicsViewer(this);
    m_scene = qobject_cast<EditGraphicsScene*>(m_viewer->scene());
    m_viewer->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_viewer->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    
    setCentralWidget(m_viewer);

    connect(m_ui->actionPrint, &QAction::triggered, this, &MainWindow::onActionPrint);
    connect(m_ui->actionAddCell, &QAction::triggered, this, &MainWindow::onActionAddCell);
    connect(m_ui->actionGenerate, &QAction::triggered, this, &MainWindow::onActionGenerate);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onActionPrint(bool checked)
{
    m_viewer->print();
}

void MainWindow::onActionAddCell(bool checked)
{
    CellItem* item = new CellItem(20, 20, 60, 60);
    m_scene->addItem(item);
}

void MainWindow::onActionGenerate(bool checked)
{
    m_scene->generateCells();
}

