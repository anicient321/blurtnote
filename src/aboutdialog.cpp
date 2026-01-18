#include "aboutdialog.h"

#include <QLabel>
#include <QDialogButtonBox>
#include <QVBoxLayout>

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("About BlurtNote"));

    QLabel *link = new QLabel;
    link->setTextFormat(Qt::RichText);
    //link->setText("<a href=\"https://github.com/anicient321/blurtnote\">https://github.com/anicient321/blurtnote</a>");
    link->setText("<a href=\"https://github.com/anicient321/blurtnote\">Our GitHub Repo</a>");
    link->setTextInteractionFlags(Qt::TextBrowserInteraction);
    link->setOpenExternalLinks(true);

    QLabel *finals = new QLabel(tr("---Finals work---"));
    auto *ime1 = new QLabel(tr("Edi Wolf (anicient321)"));
    auto *ime2 = new QLabel(tr("Filip Avelini (filipavelini)"));

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttons, &QDialogButtonBox::accepted, this, &AboutDialog::accept);

    auto *layout = new QVBoxLayout;
    layout->addWidget(link);
    layout->addWidget(finals);
    layout->addWidget(ime1);
    layout->addWidget(ime2);
    layout->addWidget(buttons);
    setLayout(layout);
}
