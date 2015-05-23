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
    count_15s_pairs_runs ();
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
        hand.suits[byIndex] = comboBoxSuits[byIndex]->currentIndex();   // Current Index will return 0-3 --> suit defined in enum

        // Need special handling for counting values
        if (hand.face_values[byIndex] <= 10) hand.count_values[byIndex] = hand.face_values[byIndex];    // Cards A-10
        else hand.count_values[byIndex] = 10;   // Face Cards
    }
}

void FormCribbageCounter::arrange_ascending (void)
/***************************
 * Arranges face values in ascending order
 **************************/
{
    unsigned char i = 0, x = 0; // counting vars
    unsigned char temp = 0;

    for (i = 0; i < 5; i++) {

        for (x = 1; x < (5 - i); x++) {

            if (hand.face_values [i] > hand.face_values [i+x]) {

                temp = hand.face_values [i]; // store larger value in temp var
                hand.face_values [i] = hand.face_values [i+x]; // move smaller value to left
                hand.face_values [i+x] = temp; // move larger value to right
            }
        }
    }

    return;
}

void FormCribbageCounter::count_15s_pairs_runs (void)
/***********************************
 * Goes through each 2, 3, 4, and 5 card combinations and sees if they add up to 15, form a pair, or form a run
 **********************************/
{
    unsigned char counter2 = 0, counter3 = 0, counter4 = 0, stop_counter = 0, i = 0; // counting vars
    unsigned char sum = 0;

    stop_counter = 5;


    for (i = 0; i < 5; i++)
    {
        for (counter2 = 1; counter2 < stop_counter; counter2++)
        {
            if (hand.count_values[i] + hand.count_values[i + counter2] == 15) score.fifteens += 2; // Combinations of 2 (10 of them)
            else if (hand.face_values[i] == hand.face_values[i + counter2]) score.pairs += 2; // Counts pairs (else if is used since 2 cards that add up to 15 cannot be the same)

            for (counter3 = 1; counter3 < (stop_counter-1); counter3++)
            {
                if (counter2+counter3+i > 4) continue; // Prevents counting past end of card array
                if (hand.count_values[i] + hand.count_values[i+counter2] + hand.count_values[i+counter2+counter3] == 15) score.fifteens += 2; // Combinations of 3 (10 of them)
                //if ((hand.count_values[i] + 1) == hand.count_values[i+counter2] && (hand.count_values[i+counter2] + 1) == hand.count_values[i+counter2+counter3]) score.runs += 3;  // Check for runs of 3
                for (counter4 = 1; counter4 < (stop_counter-2); counter4++)
                {
                    if (counter2+counter3+counter4+i > 4) continue; // Prevents counting past end of card array
                    if (hand.count_values[i] + hand.count_values[i + counter2] + hand.count_values[i+counter2+counter3] + hand.count_values[i+counter2+counter3+counter4] == 15) score.fifteens += 2; // Combinations of 4 (5 of them)
                    //if ((hand.count_values[i] + 1) == hand.count_values[i+counter2] && (hand.count_values[i+counter2] + 1) == hand.count_values[i+counter2+counter3] && (hand.count_values[i+counter2+counter3] + 1) == hand.count_values[i+counter2+counter3+counter4]) score.runs += 1;  // Check for runs of 4
                }
            }
        }

        stop_counter--;

    }

    for (i = 0; i < 5; i++) sum += hand.count_values[i]; // Counts all cards together
    if (sum == 15) score.fifteens += 2;
    //if (score.pairs == 0 && sum == (5*hand.face_values[0] + 10)) score.runs = 5;    // EXPERIMENTAL
}

void FormCribbageCounter::count_runs (void)
/******************************
 * Counts runs of 3, 4, and 5 -- arrange_ascending called to simplify this function
 *****************************/
{
    // Check for double runs that aren't checked for later
    if (((hand.face_values [0] + 1) == hand.face_values [1] && hand.face_values [1] == hand.face_values [2] && (hand.face_values [2] + 1) == hand.face_values [3])
            || ((hand.face_values [1] + 1) == hand.face_values [2] && hand.face_values [2] == hand.face_values [3] && (hand.face_values [3] + 1) == hand.face_values [4])) {

        score.runs = 6;
        return;
    }

    else if ((hand.face_values [0] + 1) == hand.face_values [1] && (hand.face_values [1] + 1) == hand.face_values [2]) { // Looks for runs starting with lowest valued card
        score.runs = 3;

        if ((hand.face_values [2] + 1) == hand.face_values [3]) { // Looks for run of 4
            score.runs = 4;

            if ((hand.face_values [3] + 1) == hand.face_values [4]) { // Looks for run of 5
                score.runs = 5;
                return;
            }

            else if (hand.face_values [3] == hand.face_values [4]) {    // Check for double runs
                score.runs = 8;
                return;
            }
        }

        else if (hand.face_values [2] == hand.face_values [3]) {    // Check for double runs
            score.runs = 6;

            if ((hand.face_values [3] + 1) == hand.face_values [4]) {
                score.runs = 8;
                return;
            }

            else if (hand.face_values [3] == hand.face_values [4]) {
                score.runs = 9;
                return;
            }
        }
    }

    else if ((hand.face_values [1] + 1) == hand.face_values [2] && (hand.face_values [2] + 1) == hand.face_values [3]) { // Looks for runs starting with second lowest valued card
        score.runs = 3;

        if ((hand.face_values [3] + 1) == hand.face_values [4]) {  // Looks for run of 4; Run of 5 not possible
            score.runs = 4;
            return;
        }

        else if (hand.face_values[0] == hand.face_values[1] || hand.face_values[3] == hand.face_values[4]) {    // Check for double runs
            score.runs = 6;
            return;
        }
    }

    else if ((hand.face_values [2] + 1) == hand.face_values [3] && (hand.face_values [3] + 1) == hand.face_values [4]) {    // Last possible run of 3
        score.runs = 3;
        if (hand.face_values[1] == hand.face_values[2]) score.runs = 6; // Check for double runs
    }

    return;
}

void FormCribbageCounter::count_flush (void)
/**********************
 * Finds if current card contains a flush
**********************/
{
    if (hand.suits[0] == hand.suits[1] && hand.suits[1] == hand.suits[2] && hand.suits[2] == hand.suits[3]) { // Checks for flush in dealt hand
        score.flushes += 4;
        if (hand.suits[0] == hand.suits[4]) score.flushes++; // Checks for 5 card flush w/ cut card
    }
}

void FormCribbageCounter::count_knobs (void)
/**************************
 * Checks for knobs
**************************/
{
    unsigned char i = 0; // Counting var

    for (i = 0; i < 4; i++) { // Go through first 4 cards and search for Jacks
        if (hand.face_values [i] == 11) { // If Jack is found, compare its suit to the cut card
            if (hand.suits [i] == hand.suits [4]) score.knobs ++;
        }
    }

    return;
}
