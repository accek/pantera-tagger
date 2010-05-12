/*  TaKIPI 1.5
 *
 *  (c) Instytut Informatyki Stosowanej Politechniki Wrocławskiej
 *  i Instytut Podstaw Informatyki PAN
 *  Autorzy: Grzegorz Godlewski, Adam Radziszewski, Maciej Piasecki,
 *  Bartosz Broda, Adam Wardyński
 *
 *  Kod na licencji GPL.
 */

#ifndef LEGACY_H
#define LEGACY_H

#include "takipi_headers.h"
#include "TaKIPI/Config.h"
#include "TaKIPI/Learning/Teacher.h"
#include "TaKIPI/Classifiers/DecisionTree.h"
#include "TaKIPI/Classifiers/DecisionTreeNode.h"
#include "TaKIPI/Classifiers/NegRuleDecisionTree.h"
#include "TaKIPI/IO.h"

#include "TaKIPI/Test.h"

int legacymain(int argc, char* argv[]);
void tag_sentence(Corpus::CSentence *s, TaKIPI::CRTTTagger &tagger, Corpus::CXmlWriter &writer, TaKIPI::CLayers &l, int &counter);

#endif
