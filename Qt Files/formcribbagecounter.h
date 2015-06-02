#ifndef FORMCRIBBAGECOUNTER_H
#define FORMCRIBBAGECOUNTER_H

#include <QMainWindow>

class QComboBox;
class QListWidget;
class QString;

enum BOOL
{
    FALSE,
    TRUE
};

enum
{
    value1,
    value2,
    value3,
    value4,
    cut_value,
    VALUE_COUNT
};

enum
{
    suit1,
    suit2,
    suit3,
    suit4,
    cut_suit,
    SUIT_COUNT
};

namespace Ui {
class FormCribbageCounter;
}

class FormCribbageCounter : public QMainWindow
{
    Q_OBJECT

public:
    explicit FormCribbageCounter(QWidget *parent = 0);
    ~FormCribbageCounter();

    typedef struct
    {
      unsigned char face_values [5];
      unsigned char count_values [5];
      unsigned char suits [5];
    } card_attributes;

    card_attributes hand;

    typedef struct
    {
      unsigned char fifteens;
      unsigned char runs;
      unsigned char pairs;
      unsigned char flushes;
      unsigned char knobs;
      unsigned char total;
    } score_types;

    score_types score;

    QComboBox *comboBoxValues [VALUE_COUNT];
    QComboBox *comboBoxSuits [SUIT_COUNT];
    QListWidget *listWidgetScores;

    void count_knobs (void);
    void count_15s_pairs (void);
    void count_runs (void);
    void count_flush (void);
    void get_hand_attributes (void);
    void arrange_ascending (void);

private slots:
    void on_pushButtonCalculate_clicked();

private:
    Ui::FormCribbageCounter *ui;
};

#endif // FORMCRIBBAGECOUNTER_H
