/**
   *******************************************
   * @file      qcptooltip.cpp
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/4
   ********************************************
   */

#include "util/qcptooltip.h"

QCPToolTip::QCPToolTip(QCustomPlot *parentPlot) :
        QCPAbstractItem(parentPlot),
        position(createPosition("position")),
        m_plot_replot(true),
        m_text_alignment(Qt::AlignLeft | Qt::AlignVCenter),
        m_radius(6, 6),
        m_size_mode(Qt::AbsoluteSize) {
    position->setType(QCPItemPosition::ptAbsolute); // static positioning in pixels
    setSelectable(false); // the tool tip can not be selected
    setLayer("overlay"); // set to "overlay" to enable fast refresh
    setBorderPen(Qt::NoPen);
    setBrush(QColor(87, 98, 93, 180)); // set the background color of the tooltip
    setTextColor(Qt::white); // set the color of the text of the tooltip
    setOffset(20, 20);
    setPadding(QMargins(6, 6, 6, 6));
    connect(mParentPlot, &QCustomPlot::mouseMove, this, &QCPToolTip::handleMouseMoveEvent);

}

void QCPToolTip::setText(const QString &text) {
    m_text = text;
}

void QCPToolTip::setFont(const QFont &font) {
    m_font = font;
}

void QCPToolTip::setTextColor(const QColor &color) {
    m_text_color = color;
}

void QCPToolTip::setBorderPen(const QPen &pen) {
    m_border_pen = pen;
}

void QCPToolTip::setBrush(const QBrush &brush) {
    m_brush = brush;
}

void QCPToolTip::setRadius(double x_radius, double y_radius, Qt::SizeMode mode) {
    m_radius = QPointF(x_radius, y_radius);
    m_size_mode = mode;
}

void QCPToolTip::setOffset(double x_offset, double y_offset) {
    m_offset = QPointF(x_offset, y_offset);
}

void QCPToolTip::setPadding(const QMargins &padding) {
    m_padding = padding;
}

void QCPToolTip::setTextAlignment(Qt::Alignment alignment) {
    m_text_alignment = alignment;
}

double QCPToolTip::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const {
    Q_UNUSED(pos);
    Q_UNUSED(onlySelectable);
    Q_UNUSED(details);
    return -1;
}

void QCPToolTip::draw(QCPPainter *painter) {
    if (m_plot_replot) {
        // called by mParentPlot, so update the position of the tooltip first, and set replot to false
        updatePosition(position->pixelPosition(), false);
        if (!visible()) return;
    }
    drawScatterPoint(painter, m_data_point);// draw customized data point
    QPointF pos = position->pixelPosition() + m_offset; // the position of the top left corner of the tooltip
    painter->translate(pos);// 将tooltip 的左上角作为当前坐标系的原点
    QFontMetrics fontMetrics(m_font);
    // 计算文字所占矩形大小
    QRect text_rect = fontMetrics.boundingRect(0, 0, 0, 0, Qt::TextDontClip | m_text_alignment, m_text);
    text_rect.moveTopLeft(QPoint(m_padding.left(), m_padding.top())); // 文字矩形区域在当前坐标系下的坐标
    QRect text_box_rect = text_rect.adjusted(-m_padding.left(), -m_padding.top(), m_padding.right(),
                                             m_padding.bottom());// 扩展文字矩形，现在矩形的左上角坐标处于原点
    text_box_rect.moveTopLeft(QPoint());

    // 限制tooltip 不超过QCustomPlot的范围
    if (pos.x() + text_box_rect.width() >= mParentPlot->axisRect()->right()) {
        // tooltip 的右侧超出边界，将其挪到鼠标左侧
        painter->translate(-m_offset.x() * 2 - text_box_rect.width(), 0);
    }
    if (pos.y() + text_box_rect.height() * 2 >= mParentPlot->axisRect()->bottom()) {
        // tooltip 的底侧超出边界，将其挪到鼠标上侧
        painter->translate(0, -m_offset.y() * 2 - text_box_rect.height());
    }
    // 绘制背景和边框
    if ((m_brush != Qt::NoBrush && m_brush.color().alpha() != 0) ||
        (m_border_pen != Qt::NoPen && m_border_pen.color().alpha() != 0)
            ) {
        double clip_pad = m_border_pen.widthF();
        QRect bounding_rect = text_box_rect.adjusted(-clip_pad, -clip_pad, clip_pad, clip_pad);//再次扩展文字矩形，使边框宽度包含在内
        painter->setPen(m_border_pen);
        painter->setBrush(m_brush);
        painter->drawRoundedRect(bounding_rect, m_radius.x(), m_radius.y(), m_size_mode); // 绘制
    }

    // 绘制文字
    painter->setFont(m_font);
    painter->setPen(m_text_color);
    painter->setBrush(Qt::NoBrush);
    painter->drawText(text_rect, Qt::TextDontClip | m_text_alignment, m_text);

}

void QCPToolTip::update() {
    m_plot_replot = false;
    layer()->replot(); // 仅让当前图层，即overlay图层绘制
    m_plot_replot = true;
}

void QCPToolTip::handleMouseMoveEvent(QMouseEvent *event) {
    updatePosition(event->pos(), true);
}
