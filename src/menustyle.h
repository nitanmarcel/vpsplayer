#ifndef MENUSTYLE_H
#define MENUSTYLE_H

#include <QProxyStyle>

class MenuStyle : public QProxyStyle
{
public:
    MenuStyle();
    int styleHint(StyleHint stylehint, const QStyleOption *opt, const QWidget *widget, QStyleHintReturn *returnData) const

    {
        if (stylehint == QStyle::SH_MenuBar_AltKeyNavigation)
            return 0;
        return QProxyStyle::styleHint(stylehint, opt, widget, returnData);
    }
};

#endif // MENUSTYLE_H
