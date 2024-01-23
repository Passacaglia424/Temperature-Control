/**
   *******************************************
   * @file      qcptooltip.h
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/4
   ********************************************
   */

#ifndef QCPTOOLTIP_H
#define QCPTOOLTIP_H

#include "util/qcustomplot.h"

/**
 * @brief a tool tip item to show data information
 */
class QCPToolTip : public QCPAbstractItem {
Q_OBJECT
public:
    QCPItemPosition *const position;

    explicit QCPToolTip(QCustomPlot *parentPlot);

    virtual ~QCPToolTip() = default;

    /**
     * @brief text to show on the tip
     * @param text
     */
    void setText(const QString &text);

    /**
     * @brief set the font of the tip
     * @param font
     */
    void setFont(const QFont &font);

    /**
     * @brief set the color of the text
     * @param color
     */
    void setTextColor(const QColor &color);

    /**
     * @brief set the pen to draw the border of the tooltip
     * @param pen
     */
    void setBorderPen(const QPen &pen);

    /**
     * @brief set the brush to draw background of the tooltip
     * @param brush
     */
    void setBrush(const QBrush &brush);

    /**
     * @brief the radius is used to draw a radius rectangle
     * @param xRadius
     * @param yRadius
     * @param mode
     */
    void setRadius(double x_radius, double y_radius, Qt::SizeMode mode = Qt::AbsoluteSize);

    /**
     * @brief distance between the tool tip and the mouse
     * @param x_offset
     * @param y_offset
     */
    void setOffset(double x_offset, double y_offset);

    /**
     * @brief padding between text and border
     * @param padding
     */
    void setPadding(const QMargins &padding);

    void setTextAlignment(Qt::Alignment alignment);

public slots:

    double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

protected:
    bool m_plot_replot; ///< 表明是由QCustomPlot刷新的，需要更新位置
    QString m_text;
    Qt::Alignment m_text_alignment;
    QFont m_font;
    QColor m_text_color;
    QPen m_border_pen;
    QBrush m_brush;
    QPointF m_radius;
    Qt::SizeMode m_size_mode;
    QPointF m_offset;
    QMargins m_padding;
    QPointF m_data_point;

    void draw(QCPPainter *painter) override;

    /**
     * @brief draw custom items on the layer except the tooltip rect
     * @param painter
     * @param pos
     */
    virtual void drawScatterPoint(QCPPainter *painter, const QPointF &pos)=0;

    void update();


    /**
     * @brief update the position of the tool tip and the data point selected
     * @param newPos
     * @param replot
     */
    virtual void updatePosition(const QPointF& newPos, bool replot=false)=0;

protected slots:
    void handleMouseMoveEvent(QMouseEvent *event);

};

#endif //QCPTOOLTIP_H
