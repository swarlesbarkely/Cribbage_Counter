#include "formcribbagecounter.h"
#include "ui_formcribbagecounter.h"

FormCribbageCounter::FormCribbageCounter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FormCribbageCounter)
{
    ui->setupUi(this);

    // Link combo boxes to each card
    comboBoxValues[value1] = findChild<QComboBox*>("comboBoxCard1");
    comboBoxValues[value2] = findChild<QComboBox*>("comboBoxCard2");
    comboBoxValues[value3] = findChild<QComboBox*>("comboBoxCard3");
    comboBoxValues[value4] = findChild<QComboBox*>("comboBoxCard4");
    comboBoxValues[cut_value] = findChild<QComboBox*>("comboBoxCutCard");

    comboBoxSuits[suit1] = findChild<QComboBox*>("comboBoxSuit1");
    comboBoxSuits[suit2] = findChild<QComboBox*>("comboBoxSuit2");
    comboBoxSuits[suit3] = findChild<QComboBox*>("comboBoxSuit3");
    comboBoxSuits[suit4] = findChild<QComboBox*>("comboBoxSuit4");
    comboBoxSuits[cut_suit] = findChild<QComboBox*>("comboBoxCutSuit");

    //  Score List
    listWidgetScores = findChild<QListWidget*>("listWidgetScores");
}

FormCribbageCounter::~FormCribbageCounter()
{
    delete ui;
}

void FormCribbageCounter::on_pushButtonCalculate_clicked()
{
    QString sTemp;

    // Init Scores to 0
    score.fifteens = 0;
    score.flushes = 0;
    score.knobs = 0;
    score.pairs = 0;
    score.runs = 0;

    get_hand_attributes ();
    count_knobs (); // Must be run before rearranging cards to keep track of cut card
    arrange_ascending ();
    count_15s_pairs ();
    count_runs ();
    count_flush ();

    score.total = score.fifteens + score.flushes + score.knobs + score.pairs + score.runs;

    // Remove old scores
    listWidgetScores->clear();

    // Print scores
    sTemp = QString("Score from 15s:  %1").arg(score.fifteens, 0, 10);
    listWidgetScores->addItem(sTemp);

    sTemp = QString("Score from pairs:  %1").arg(score.pairs, 0, 10);
    listWidgetScores->addItem(sTemp);

    sTemp = QString("Score from runs:  %1").arg(score.runs, 0, 10);
    listWidgetScores->addItem(sTemp);

    sTemp = QString("Score from flushes:  %1").arg(score.flushes, 0, 10);
    listWidgetScores->addItem(sTemp);

    sTemp = QString("Score from knobs:  %1").arg(score.knobs, 0, 10);
    listWidgetScores->addItem(sTemp);

    sTemp = QString("\nTotal Score:  %1").arg(score.total, 0, 10);
    listWidgetScores->addItem(sTemp);
}

void FormCribbageCounter::get_hand_attributes (void)
{
    unsigned char byIndex;

    for (byIndex = 0; byIndex < VALUE_COUNT; byIndex++)
    {
        hand.face_values[byIndex] = comboBoxValues[byIndex]->currentIndex() + 1;    // Current Index + 1 will return 1-13
        hand.suits[byIndex] = comboBoxSuits[byIndex]->currentIndex();   // Current Index will return 0-3 --> actual suit doesn't matter, only whether or not they are the same

        // Need special handling for counting values
        if (hand.face_values[byIndex] <= 10)    // Cards A-10
        {
            hand.count_values[byIndex] = hand.face_values[byIndex];
        }

        else hand.count_values[byIndex] = 10;   // Face Cards
    }
}

void FormCribbageCounter::arrange_ascending (void)
/********************************************************************************
 * Arranges face values in ascending order
 ********************************************************************************/
{
    unsigned char  i;
    unsigned char  x;
    unsigned char  temp = 0;

    for (i = 0; i < 5; i++) {

        for (x = 1; x < (5 - i); x++)
        {
            if (hand.face_values [i] > hand.face_values [i+x])
            {
                temp = hand.face_values [i]; // store larger value in temp var
                hand.face_values [i] = hand.face_values [i+x]; // move smaller value to left
                hand.face_values [i+x] = temp; // move larger value to right
            }
        }
    }

    return;
}

void FormCribbageCounter::count_15s_pairs (void)
/*********************************************************************************
 * Goes through each 2, 3, 4, and 5 card combinations and sees if they add up to
 * 15 or form a pair
 *********************************************************************************/
{
    unsigned char  counter2;
    unsigned char  counter3;
    unsigned char  counter4;
    unsigned char  stop_counter;
    unsigned char  i;
    unsigned char  sum = 0;

    stop_counter = 5;


    for (i = 0; i < 5; i++)
    {
        for (counter2 = 1; counter2 < stop_counter; counter2++)
        {
            if (hand.count_values[i] + hand.count_values[i + counter2] == 15)   // Combinations of 2 (10 of them)
            {
                score.fifteens += 2;
            }

            else if (hand.face_values[i] == hand.face_values[i + counter2]) // Counts pairs (else if is used since 2 cards that add up to 15 cannot be the same)
            {
                score.pairs += 2;
            }

            for (counter3 = 1; counter3 < (stop_counter-1); counter3++)
            {
                if (counter2+counter3+i > 4)
                {
                    continue; // Prevents counting past end of card array
                }

                if (hand.count_values[i] + hand.count_values[i+counter2] + hand.count_values[i+counter2+counter3] == 15) // Combinations of 3 (10 of them)
                {
                    score.fifteens += 2;
                }

                for (counter4 = 1; counter4 < (stop_counter-2); counter4++)
                {
                    if (counter2+counter3+counter4+i > 4)
                    {
                        continue; // Prevents counting past end of card array
                    }

                    if (hand.count_values[i] + hand.count_values[i + counter2] + hand.count_values[i+counter2+counter3] + hand.count_values[i+counter2+counter3+counter4] == 15) // Combinations of 4 (5 of them)
                    {
                        score.fifteens += 2;
                    }
                }
            }
        }

        stop_counter--;

    }

    for (i = 0; i < 5; i++) sum += hand.count_values[i]; // Counts all cards together
    if (sum == 15) score.fifteens += 2;
}

void FormCribbageCounter::count_runs (void)
/*********************************************************************************
 * Counts runs of 3, 4, and 5 -- arrange_ascending called to simplify this function
 *********************************************************************************/
{
    unsigned char  byStartingCard = 0;
    unsigned char  byMultiplier;
    unsigned char  byIndex;
    unsigned char  byCurrentRun;

    for (byStartingCard = 0; byStartingCard <= 3; byStartingCard++)
    {
        byMultiplier = 1;
        byIndex = 1;
        byCurrentRun = 1;

        while (TRUE)
        {
            if (hand.face_values[byStartingCard] + byCurrentRun == hand.face_values[byStartingCard + byIndex])
            {
                byCurrentRun++;
                byIndex++;
            }

            else if (hand.face_values[byStartingCard + byCurrentRun - 1] == hand.face_values[byStartingCard + byIndex])
            {
                byMultiplier++;
                byIndex++;
            }

            else
            {
                if (hand.face_values[byStartingCard + byIndex] == hand.face_values[byStartingCard + byIndex - 1])
                {
                    byMultiplier *= 2;
                }

                break;
            }
        }

        if (byCurrentRun >= 3)
        {
            score.runs = (byCurrentRun*byMultiplier);
            return;
        }
    }

    score.runs = 0;

}

void FormCribbageCounter::count_flush (void)
/*******************************************************
 * Finds if current hand contains a flush
*******************************************************/
{
    if (hand.suits[0] == hand.suits[1] && hand.suits[1] == hand.suits[2] && hand.suits[2] == hand.suits[3]) // Checks for flush in dealt hand
    {
        score.flushes += 4;

        if (hand.suits[0] == hand.suits[4])  // Checks for 5 card flush w/ cut card
        {
            score.flushes++;
        }
    }
}

void FormCribbageCounter::count_knobs (void)
/******************************************************
 * Checks for knobs
******************************************************/
{
    unsigned char  i = 0; // Counting var

    for (i = 0; i < 4; i++) // Go through first 4 cards and search for Jacks
    {
        if (hand.face_values [i] == 11) // If Jack is found, compare its suit to the cut card
        {
            if (hand.suits [i] == hand.suits [4]) score.knobs ++;
        }
    }
}

