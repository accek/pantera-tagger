/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef TAKIPI_TAKIPI_ALL_H
#define TAKIPI_TAKIPI_ALL_H

#include "UDictionary.h"
#include "CutOff.h"
#include "LexemAscendingSort.h"
#include "Config.h"
#include "IO.h"
#include "PermutationGenerator.h"
#include "Tagger.h"
#include "Layers.h"

#include "Learning/Filter.h"
#include "Learning/Teacher.h"


#include "Classifiers/Classifier.h"
#include "Classifiers/BagClassifier.h"
#include "Classifiers/ClassifierRule.h"
#include "Classifiers/DecisionTree.h"
#include "Classifiers/DecisionTreeNode.h"
#include "Classifiers/LMTree.h"
#include "Classifiers/LMTreeNode.h"
#include "Classifiers/LogisticModel.h"
#include "Classifiers/NegRuleDecisionTree.h"
#include "Classifiers/NegRuleDecisionTreeNode.h"
#include "Classifiers/Ripper.h"
#include "Classifiers/RuleClassifier.h"
#include "Classifiers/VotingAverageClassifier.h"
#include "Classifiers/VotingWTAClassifier.h"
#include "Classifiers/Part.h"

#endif
