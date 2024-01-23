/**
   *******************************************
   * @file      plot_widget.cpp
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/1
   ********************************************
   */
#include "QVBoxLayout"
#include "QSpinBox"
#include "QCheckBox"
#include "QRadioButton"
#include "QButtonGroup"
#include "QMenu"
#include "QDebug"
#include "util/color.h"
#include "util/plot_widget.h"
#include "util/qcptooltip_graph.h"


#define AXIS_PTR(axis, ptr) \
switch (axis){         \
case XBottom:        \
ptr=m_custom_plot->xAxis; \
break;               \
case XTop:           \
ptr=m_custom_plot->xAxis2;\
break;               \
case YLeft:          \
ptr=m_custom_plot->yAxis; \
break;               \
case YRight:         \
ptr=m_custom_plot->yAxis2;\
break;               \
}\


static void setAxisLabelFont(QFont &font) {
    font.setBold(AXIS_LABEL_FONT_BOLD);
    font.setPointSize(AXIS_LABEL_SIZE);
    font.setFamily(AXIS_LABEL_FONT_FAMILY);
}

static void setAxisTickLabelFont(QFont &font) {
    font.setPointSize(AXIS_TICK_LABEL_SIZE);
    font.setFamily(AXIS_TICK_LABEL_FONT_FAMILY);
}

static void setTitleFont(QFont &font) {
    font.setBold(true);
    font.setPixelSize(18);
}

BasicPlotWidget::BasicPlotWidget(QWidget *parent) :
        QWidget(parent),
        m_custom_plot(new QCustomPlot(this)),
        m_context_menu(new QMenu(this)) {

    auto rescale_action = new QAction("ReScale");
    m_context_menu->addAction(rescale_action);
    m_custom_plot->setContextMenuPolicy(Qt::CustomContextMenu);
    m_context_menu->setWindowFlags(
            m_context_menu->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    m_context_menu->setAttribute(Qt::WA_TranslucentBackground);
    connect(rescale_action, &QAction::triggered, this, [this]() {
        reScaleAxes();
        replot(QCustomPlot::RefreshPriority::rpQueuedReplot);
    });
    auto layout = new QGridLayout(this);
    layout->addWidget(m_custom_plot);
    layout->setContentsMargins(0, 0, 0, 0);
    setContentsMargins(0, 0, 0, 0);
    connect(m_custom_plot->xAxis, SIGNAL(rangeChanged(
                                                 const QCPRange&)), m_custom_plot->xAxis2,
            SLOT(setRange(
                         const QCPRange &)));
    connect(m_custom_plot->yAxis, SIGNAL(rangeChanged(
                                                 const QCPRange &)), m_custom_plot->yAxis2,
            SLOT(setRange(
                         const QCPRange &)));

    QFont axis_font;
    setAxisLabelFont(axis_font);
    setLabelFont(XBottom, axis_font);
    setLabelFont(YLeft, axis_font);
    QFont axis_tick_font;
    setAxisTickLabelFont(axis_tick_font);
    setTickLabelFont(XBottom, axis_tick_font);
    setTickLabelFont(YLeft, axis_tick_font);

    setAxisVisible(XTop, true);
    setAxisVisible(YRight, true);
    setTickLabelVisible(XTop, false);
    setTickLabelVisible(YRight, false);
//    m_custom_plot->setOpenGl(true);

}

void BasicPlotWidget::reScaleAxes() {
    m_custom_plot->rescaleAxes();
}

void BasicPlotWidget::setAxesRange(BasicPlotWidget::Axis axis, const QCPRange &range) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setRange(range);
}

void BasicPlotWidget::replot(QCustomPlot::RefreshPriority refreshPriority) {
    m_custom_plot->replot(refreshPriority);
}

void BasicPlotWidget::setLabel(Axis axis, const QString &label) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setLabel(label);
}

void BasicPlotWidget::setAxisVisible(BasicPlotWidget::Axis axis, bool visible) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setVisible(visible);
}

void BasicPlotWidget::setTickLabelVisible(BasicPlotWidget::Axis axis, bool visible) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setTickLabels(false);
}

void BasicPlotWidget::setLegendVisible(bool visible) {
    m_custom_plot->legend->setVisible(visible);
}

void BasicPlotWidget::setLabelColor(BasicPlotWidget::Axis axis, const QColor &color) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setLabelColor(color);
}

void BasicPlotWidget::setLabelFont(BasicPlotWidget::Axis axis, const QFont &font) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setLabelFont(font);
}

void BasicPlotWidget::setTitle(const QString &title, const QFont &font) {
    m_custom_plot->plotLayout()->insertRow(0);
    m_custom_plot->plotLayout()->addElement(0, 0, new QCPTextElement(m_custom_plot, title, font));
}

void BasicPlotWidget::setGridVisible(bool visible) {
    m_custom_plot->xAxis->grid()->setVisible(visible);
    m_custom_plot->yAxis->grid()->setVisible(visible);
}

bool BasicPlotWidget::saveFigure(const QString &file_name, BasicPlotWidget::FigureFormat format, int dpi) {
    switch (format) {
        case PDF:
            return m_custom_plot->savePdf(file_name);
        case PNG:
            return m_custom_plot->savePng(file_name, 0, 0, 1, 100, dpi);
        case BMP:
            return m_custom_plot->saveBmp(file_name, 0, 0, 1, dpi);
        case JPG:
            return m_custom_plot->saveJpg(file_name, 0, 0, 1, 100, dpi);
        default:
            return false;
    }
}

void BasicPlotWidget::setContextMenu(const std::function<void(QPoint)> &slot) {
    connect(m_custom_plot, &QCustomPlot::customContextMenuRequested, this, slot);
}

QCPRange BasicPlotWidget::getRange(BasicPlotWidget::Axis axis) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    return ax->range();
}

void BasicPlotWidget::setRange(BasicPlotWidget::Axis axis, const QCPRange &range) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setRange(range);
}

QFont BasicPlotWidget::defaultTitleFont() {
    QFont font;
    font.setBold(true);
    font.setPixelSize(20);
    return font;
}

void BasicPlotWidget::setTitle(const QString &title) {
    setTitle(title, defaultTitleFont());
}

void BasicPlotWidget::setTickLabelFont(BasicPlotWidget::Axis axis, const QFont &font) {
    QCPAxis *ax;
    AXIS_PTR(axis, ax)
    ax->setTickLabelFont(font);
}

void BasicPlotWidget::setLegendFont(const QFont &font) {
    m_custom_plot->legend->setFont(font);
}


GraphWidget::GraphWidget(QWidget *parent) : BasicPlotWidget(parent), m_tool_tip(nullptr) {
    m_tool_tip = new QCPGraphToolTip(m_custom_plot);
    setContextMenu([this](QPoint pos) {
        if (m_custom_plot->axisRect()->selectTest(pos, false) > 0) {
            m_context_menu->exec(m_custom_plot->mapToGlobal(pos));
        }
    });
    m_custom_plot->setPlottingHints(QCP::phFastPolylines | QCP::phCacheLabels);
}

void GraphWidget::addGraph(Axis keyAxis, Axis valueAxis) {
    QCPAxis *key_axis;
    QCPAxis *val_axis;
    AXIS_PTR(keyAxis, key_axis)
    AXIS_PTR(valueAxis, val_axis)
    m_custom_plot->addGraph(key_axis, val_axis);
    setDefaultStyle(m_custom_plot->graph(m_custom_plot->graphCount() - 1));
}

void GraphWidget::removeGraph(int index) {
    m_custom_plot->removeGraph(index);
}

void GraphWidget::setPen(int index, const QPen &pen) {
    m_custom_plot->graph(index)->setPen(pen);
}

void GraphWidget::setBrush(int index, const QBrush &brush) {
    m_custom_plot->graph(index)->setBrush(brush);
}

void GraphWidget::setLineStyle(int index, QCPGraph::LineStyle lineStyle) {
    m_custom_plot->graph(index)->setLineStyle(lineStyle);
}

void GraphWidget::setName(int index, const QString &name) {
    m_custom_plot->graph(index)->setName(name);
}

void GraphWidget::setScatterStyle(int index, const QCPScatterStyle &scatterStyle) {
    m_custom_plot->graph(index)->setScatterStyle(scatterStyle);
}

void GraphWidget::setErrorBar(int index, QCPErrorBars::ErrorType errorType) {
    if (m_error_bars.find(index) != m_error_bars.end()) {
        m_error_bars[index]->setErrorType(errorType);
        return;
    }
    auto error_bar = new QCPErrorBars(m_custom_plot->xAxis, m_custom_plot->yAxis);
    error_bar->setDataPlottable(m_custom_plot->graph(index));
    error_bar->removeFromLegend();
    error_bar->setErrorType(errorType);
    m_error_bars.insert(index, error_bar);
}

void GraphWidget::setData(int index, const QVector<double> &x_data, const QVector<double> &y_data, bool is_sorted) {
    m_custom_plot->graph(index)->setData(x_data, y_data, is_sorted);
}

void GraphWidget::setData(int index, const QVector<double> &x_data, const QVector<double> &y_data,
                          const QVector<double> &y_error, bool is_sorted) {
    m_custom_plot->graph(index)->setData(x_data, y_data, is_sorted);
    if (m_error_bars.find(index) != m_error_bars.end()) {
        m_error_bars[index]->setData(y_error);
    }
}

void GraphWidget::appendData(int index, const QVector<double> &x_data, const QVector<double> &y_data, bool is_sorted) {
    m_custom_plot->graph(index)->addData(x_data, y_data, is_sorted);
}

void GraphWidget::appendData(int index, const QVector<double> &x_data, const QVector<double> &y_data,
                             const QVector<double> &y_error, bool is_sorted) {
    m_custom_plot->graph(index)->addData(x_data, y_data, is_sorted);
    if (m_error_bars.find(index) != m_error_bars.end()) {
        m_error_bars[index]->addData(y_error);
    }
}

void GraphWidget::appendData(int index, double x_data, double y_data) {
    m_custom_plot->graph(index)->addData(x_data, y_data);
}

void GraphWidget::appendData(int index, double x_data, double y_data, double y_error) {
    m_custom_plot->graph(index)->addData(x_data, y_data);
    if (m_error_bars.find(index) != m_error_bars.end()) {
        m_error_bars[index]->addData(y_error);
    }
}

void GraphWidget::removeData(int index, double begin, double end) {
    m_custom_plot->graph(index)->data()->remove(begin, end);
}

void GraphWidget::removeDataByIndex(int index, int begin, int end) {
    auto graph = m_custom_plot->graph(index);
    auto coreData = graph->data()->coreData();
    if (begin >= coreData->size() || end >= coreData->size() || begin >= end) {
        return;
    }
    coreData->erase(coreData->cbegin() + begin, coreData->cbegin() + end);
}

void GraphWidget::setData(int index, int x_id, double y) {
    (m_custom_plot->graph(index)->data()->begin() + x_id)->value = y;
}

void GraphWidget::setData(int index, int x_id, double y, double y_error) {
    setData(index, x_id, y);
    if (m_error_bars.find(index) != m_error_bars.end()) {
        (m_error_bars[index]->data()->begin() + x_id)->errorMinus = y_error;
        (m_error_bars[index]->data()->begin() + x_id)->errorPlus = y_error;
    }
}

void GraphWidget::clearData(int index) {
    auto graph = m_custom_plot->graph(index);
    graph->data()->clear();
}

void GraphWidget::setLayer(int index, const QString &layer) {
    m_custom_plot->graph(index)->setLayer(layer);
}

void GraphWidget::setAdaptiveSampling(int index, bool enable) {
    m_custom_plot->graph(index)->setAdaptiveSampling(enable);
}

void GraphWidget::setDefaultStyle(QCPGraph *graph) {
    graph->setLineStyle(QCPGraph::lsLine);
    graph->setAdaptiveSampling(true);

}
