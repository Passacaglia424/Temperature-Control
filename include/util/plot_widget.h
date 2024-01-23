/**
   *******************************************
   * @file      plot_widget.h
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/1
   ********************************************
   */

#ifndef PLOT_WIDGET_H
#define PLOT_WIDGET_H

#include "QWidget"
#include "QObject"
#include "QMenu"
#include "QRadioButton"

//#define QCUSTOMPLOT_USE_OPENGL

#include "util/qcustomplot.h"
#include "qcptooltip.h"


/**
 * @brief 绘图控件基类
 * @details 提供以下功能：
 * 1. 调整坐标轴范围
 * 2. 设置坐标轴范围
 * 3. 绘制图像
 * 4. 设置坐标轴标签
 * 5. 设置图例
 * 6. 设置标题
 * 7. 设置网格
 * 8. 保存图片
 * 9.
 * @note
 * 一个BasicPlotWidget 包含一个坐标区域
 */
class BasicPlotWidget : public QWidget {
Q_OBJECT

public:
    enum FigureFormat {
        JPG,
        PNG,
        PDF,
        BMP
    };

    enum Axis {
        XBottom,
        YLeft,
        XTop,
        YRight
    };

    explicit BasicPlotWidget(QWidget *parent = nullptr);

    ~BasicPlotWidget() override = default;

    /**
     * @brief 调整坐标轴范围以显示全部数据点
     * @note 不会刷新绘图
     */
    virtual void reScaleAxes();


    /**
     * @brief 设置坐标轴范围
     * @param range
     */
    virtual void setAxesRange(Axis axis, const QCPRange &range);

    /**
     * @brief 刷新绘图区以应用设置
     */
    virtual void replot(QCustomPlot::RefreshPriority refreshPriority = QCustomPlot::RefreshPriority::rpQueuedReplot);

    /**
     * @brief 设置坐标轴标签
     * @param axis
     * @param label
     */
    void setLabel(Axis axis, const QString &label);

    /**
     * @brief 设置坐标轴标签颜色
     * @param axis
     * @param color
     */
    void setLabelColor(Axis axis, const QColor &color);

    /**
     * @brief 设置坐标轴标签字体
     * @param axis
     * @param font
     */
    void setLabelFont(Axis axis, const QFont &font);

    /**
     * @brief 设置坐标轴是否显示
     * @param axis
     * @param visible
     */
    void setAxisVisible(Axis axis, bool visible);

    /**
     * @brief 是否显示刻度标签
     * @param axis
     * @param visible
     */
    void setTickLabelVisible(Axis axis, bool visible);

    void setTickLabelFont(Axis axis, const QFont &font);

    /**
     * @brief 设置图例是否显示
     * @param visible
     */
    void setLegendVisible(bool visible);

    void setLegendFont(const QFont &font);


    /**
     * @brief 设置标题
     * @param title
     */
    void setTitle(const QString &title, const QFont &font);

    void setTitle(const QString &title);

    static QFont defaultTitleFont();

    /**
     * @brief 是否显示网格
     * @param visible
     */
    void setGridVisible(bool visible);

    /**
     * @brief 保存图片
     * @param file_name
     * @param format
     * @param dpi
     */
    bool saveFigure(const QString &file_name, FigureFormat format, int dpi);

    QCPRange getRange(Axis axis);

    void setRange(Axis axis, const QCPRange &range);

    QCustomPlot *m_custom_plot;

signals:

    void sigSelectionChanged(bool);

    void sigSelectionChanged(double x, double y);

protected:

    void setContextMenu(const std::function<void(QPoint)> &slot);

};

/**
 * @brief 折线图控件
 */
class GraphWidget : public BasicPlotWidget {
Q_OBJECT

public:
    explicit GraphWidget(QWidget *parent = nullptr);

    ~GraphWidget() override = default;

    /**
     * @brief 在坐标轴上新增一条曲线
     */
    void addGraph(Axis keyAxis = XBottom, Axis valueAxis = YLeft);

    /**
     * @brief 移除第 index 条曲线
     * @param index
     */
    void removeGraph(int index);

    /**
     * @brief 设置曲线颜色
     * @param index 第 index 条曲线
     * @param pen
     */
    void setPen(int index, const QPen &pen);

    /**
     * @brief 设置曲线与坐标轴之间的填充色
     * @param index
     * @param brush
     */
    void setBrush(int index, const QBrush &brush);

    /**
     * @brief 设置曲线线型
     * @param index
     * @param lineStyle
     */
    void setLineStyle(int index, QCPGraph::LineStyle lineStyle);

    /**
     * @brief 设置曲线在图例中的名称
     * @param index
     * @param name
     */
    void setName(int index, const QString &name);

    /**
     * @brief 设置曲线点型
     * @param index
     * @param scatterStyle
     */
    void setScatterStyle(int index, const QCPScatterStyle &scatterStyle);

    /**
     * @brief 将曲线设置成带有 error bar 的线型
     * @param index
     */
    void setErrorBar(int index, QCPErrorBars::ErrorType errorType);

    /**
     * @brief 设置曲线数据
     * @param index
     * @param x_data
     * @param y_data
     * @param is_sorted x 是否已经升序排列
     */
    void setData(int index, const QVector<double> &x_data, const QVector<double> &y_data, bool is_sorted = false);

    /**
     * @brief 设置带 error bar 的数据
     * @param index
     * @param x_data
     * @param y_data
     * @param y_error
     * @param is_sorted
     */
    void
    setData(int index, const QVector<double> &x_data, const QVector<double> &y_data, const QVector<double> &y_error,
            bool is_sorted = false);

    /**
     * @brief 向曲线中追加数据
     * @param index
     * @param x_data
     * @param y_data
     * @param is_sorted
     */
    void appendData(int index, const QVector<double> &x_data, const QVector<double> &y_data, bool is_sorted = false);

    /**
     * @brief 向曲线中追加带 error bar 的数据
     * @param index
     * @param x_data
     * @param y_data
     * @param y_error
     */
    void
    appendData(int index, const QVector<double> &x_data, const QVector<double> &y_data, const QVector<double> &y_error,
               bool is_sorted = false);

    /**
     * @brief 向曲线中追加数据
     * @param index
     * @param x_data
     * @param y_data
     */
    void appendData(int index, double x_data, double y_data);

    /**
     * @brief 向曲线中追加带 error bar的数据
     * @param index
     * @param x_data
     * @param y_data
     * @param y_error
     */
    void appendData(int index, double x_data, double y_data, double y_error);

    /**
     * @brief 移除曲线中指定范围内的数据，[begin,end)
     * @param index
     * @param begin 横坐标起始
     * @param end 横坐标结束
     */
    void removeData(int index, double begin, double end);

    /**
     * @brief 移除曲线中指定范围内的数据, [begin, end)
     * @param index
     * @param begin
     * @param end
     */
    void removeDataByIndex(int index, int begin, int end);

    /**
     * @brief 设置 曲线中某个点的y值
     * @param index 曲线id
     * @param x_id 第 x_id 个点
     * @param y 新的 y 值
     */
    void setData(int index, int x_id, double y);

    /**
     * @brief 设置曲线中某个点y值和error bar
     * @param index
     * @param x_id
     * @param y
     * @param y_error
     */
    void setData(int index, int x_id, double y, double y_error);

    /**
     * clear all data in the graph
     * @param index index of the graph
     */
    void clearData(int index);

    void setLayer(int index, const QString &layer);

    void setAdaptiveSampling(int index, bool enable);

private:
    QCPToolTip *m_tool_tip;
    QMap<int, QCPErrorBars *> m_error_bars;

    static void setDefaultStyle(QCPGraph *graph);
};

#endif //PLOT_WIDGET_H
