// TODO poprawić formatowanie
!casstd (
   flex[-3], flex[-2], flex[-1], flex[0], flex[1], flex[2],
   cas[-3], cas[-2], cas[-1],       cas[1], cas[2],
   gnd[-3], gnd[-2], gnd[-1], gnd[0], gnd[1], gnd[2],
   nmb[-3], nmb[-2], nmb[-1], nmb[0], nmb[1], nmb[2]
)

!CaseBeforeLeftVerb (
	?cas[$C]?and(
		not(
			in(flex[-1],{fin,praet,bedzie})
		),
		llook(-2,begin,$V,in(flex[$V],{fin,praet,bedzie})),
		llook(-1,$+1V,$C,
			not(equal(cas[$C],{none}))
		)
	)
)

!NearestVerbLeft (
	?base[$V]?or(
		llook(0,-1,$V,in(flex[$V],{fin,praet,bedzie,pact,ppas,pcon,pant,ger})),
		and(
			llook(-2,begin,$V,in(flex[$V],{fin,praet,bedzie})),
			only($+1V,-1,$i,
				not(inter(flex[$i],{fin,praet,bedzie}))
			)
		)
	)
)

!NearestVerbRight (
	?base[$V]?or(
		rlook(0,1,$V,in(flex[$V],{fin,praet,bedzie})),
		and(
			rlook(-2,begin,$V,in(flex[$V],{fin,praet,bedzie})),
			only(1,$-1V,$i,
				not(inter(flex[$i],{fin,praet,bedzie}))
			)
		)
	)
)

!AfterPrepWeak (
	?cas[$P]?and(
			//in(flex[0],{adj,ger,num,pact,ppas,subst}),
		not(
			equal(flex[0],{prep})
		),
		or(
			and(
				llook(0,-1,$P,equal(flex[$P],{prep})),
				inter(cas[0],cas[$P])
			),
			and(
				llook(-2,begin,$P,equal(flex[$P],{prep})),
				inter(cas[0],cas[$P]),
				only($+1P,-1,$PL, in(flex[$PL],{adj,pact,ppas})),
				wagr($+1P,0,{cas,nmb,gnd},3)
			)
		)
	)
)

!PPAfterPrepWeak (
	?cas[$Sbr]?and(
		equal(flex[0],{prep}),
		or(
			and(
				rlook(0,1,$Sbr,in(flex[$Sbr],{ger,subst})),
				inter(cas[0],cas[$Sbr])
			),
			and(
				rlook(2,end,$Sbr,in(flex[$Sbr],{ger,subst})),
				inter(cas[0],cas[$Sbr]),
				only(1,$-1Sbr,$PR, in(flex[$PR],{adj,pact,ppas})),
				wagr(1,$Sbr,{cas,nmb,gnd},3)
			)
		)
	)
)

!PPAfterPrepStrict (
	?cas[$Sbr]?and(
		equal(flex[0],{prep}),
		or(
			and(
				rlook(0,1,$Sbr,in(flex[$Sbr],{ger,subst})),
				inter(cas[0],cas[$Sbr])
			),
			and(
				rlook(2,end,$Sbr,in(flex[$Sbr],{ger,subst})),
				inter(cas[0],cas[$Sbr]),
				only(1,$-1Sbr,$PR, in(flex[$PR],{adj})),
				wagr(1,$Sbr,{cas,nmb,gnd},3)
			)
		)
	)
)

!AgrAcrossConjShort (
   ?cas[$C]?or(
     and(
       in(base[-1],{"albo","czy","i","lub","oraz"}),
       not(
           equal(cas[-2],{none})
       ),
       llook(-2,-2,$C,not(
                          equal(cas[$C],{none})
                      )), //sztuczne ustawienie zmiennej
       inter(cas[0],cas[$C])
      ),
      and(
       in(base[1],{"albo","czy","i","lub","oraz"}),
       not(
           equal(cas[2],{none})
           ),
       rlook(2,2,$C,not(
                          equal(cas[$C],{none})
                      )), //sztuczne ustawienie zmiennej
       inter(cas[0],cas[$C])
     )
   )
)

!PossibleSubj (
  and(
   not(
          equal(flex[0],{ppron12}),
          and(
           equal(flex[-1],{prep}),
           inter(cas[-1],cas[0])
          )
   ),
   or(
    and(
        rlook(1,end,$Fin,and(
                             inter(flex[$Fin],{bedzie,fin,praet}),
                             not(
                               inter(per[$Fin],{pri,sec})
                             ),
                             inter(nmb[$Fin],nmb[0]),
                             or(
                                in(flex[$Fin],{bedzie,fin}),
                                and(
                                  agrpp(0,$Fin,{nmb,gnd},2)
                                )
                             )
                          )),
        only(1,$Fin,$Cs,not(
                          inter(cas[$Cs],{nom})
	   	          )),
        or(
           llook(-1,begin,$Sb,not(
                               inter(cas[$Sb],{nom,none})
		              )),
	   and(
	     llook(-1,begin,$Sb,inter(cas[$Sb],{nom})),
             llook(-1,$Sb,$LF,inter(flex[$LF],{conj,bedzie,fin,praet}))
	  )
       )
    ),
    and(
        llook(-1,begin,$Fin,and(
                             inter(flex[$Fin],{bedzie,fin,praet}),
                             not(
                               inter(per[$Fin],{pri,sec})
                             ),
                             inter(nmb[$Fin],nmb[0]),
                             or(
                                in(flex[$Fin],{bedzie,fin}),
                                and(
                                  agrpp(0,$Fin,{nmb,gnd},2)
                                )
                             )
                          )),
        only($Fin,-1,$Csl,not(
                            inter(cas[$Csl],{nom})
		           )),
        or(
           rlook(1,end,$Sbl,not(
                               inter(cas[$Sbl],{nom,none})
		              )),
	   and(
	     rlook(1,end,$Sbl,inter(cas[$Sbl],{nom})),
             rlook(1,$Sbl,$LFl,inter(flex[$LFl],{conj,bedzie,fin,praet}))
	  )
       )
    )
   )
  )
)

!AgrAcrossConjLongAcc (
   or(
     and(
        or(  //jest conj i do niego wszystko uzgodnione
          llook(0,-1,$C,and(
                           equal(flex[$C],{conj}),
                           in(base[$C],{"albo","czy","i","lub","oraz"})
	        	)),
	  and(
	    llook(-2,begin,$C,and(
                              equal(flex[$C],{conj}),
                              in(base[$C],{"albo","czy","i","lub","oraz"})
		  	    )),
  	    only($+1C,-1,$OBC,not(
	                        equal(cas[$OBC],{none})
	                      )),
	    wagr($+1C,0,{acc,nmb,gen},3)
	  )
        ),
        or(     //szukamy spójnej frazy po drugiej stronie conj
           equal(cas[$-1C],{acc}),
	   and(
             llook($-1C,begin,$A, equal(cas[$A],{acc})),
	     only($A,$-1C,$OAC,not(
	                          equal(cas[$OAC],{none})
	                       )),
             wagr($A,$-1C,{acc,nmb,gen},3)
	   )
        )
     ),
     and(
        or(
          rlook(0,1,$Cr,and(
                           equal(flex[$Cr],{conj}),
                           in(base[$Cr],{"albo","czy","i","lub","oraz"})
	        	)),
	  and(
	    rlook(2,end,$Cr,and(
                              equal(flex[$Cr],{conj}),
                              in(base[$Cr],{"albo","czy","i","lub","oraz"})
		  	    )),
  	    only(1,$-1C,$OBCr,not(
	                        equal(cas[$OBCr],{none})
	                      )),
	    wagr(0,$-1C,{acc,nmb,gen},3)
	  )
        ),
        or(
           equal(cas[$+1C],{acc}),
	   and(
             rlook($+1C,end,$Ar, equal(cas[$Ar],{acc})),
	     only($+1C,$Ar,$OACr,not(
	                          equal(cas[$OACr],{none})
	                       )),
             wagr($+1C,$A,{acc,nmb,gen},3)
	   )
        )
     )
   )
)
   
!AfterConjJako (
   and(
     not(
       equal(flex[0],{prep})
     ),
     or(
       equal(orth[-1],{"jako"}),
       and(
         llook(-2,begin,$C,equal(orth[$C],{"jako"})),
         only($+1C,-1,$O,in(flex[$O],{adj,ppas,pact})),
         wagr($+1C,0,{acc,nmb,gnd},3)
       )
     )
   )
)

!NieMa (
	and(
		in(orth[-2],{"nie","Nie","NIE"}),
		in(orth[-1],{"ma","było","będzie"})
	)
)

// zwraca zbiór przypadków spełniający uzgodnienie wokół koordynacji
!CoordCase (
	? agrflt($BeforeConj,$AfterConj,{cas,nmb,gnd},3,{cas}) ? and(
		or(
			and(
				// szukamy po lewej
				llook(-2,-5,$BeforeConj,and(
					inter(flex[$+1BeforeConj],{conj}),
					in(orth[$+1BeforeConj],{"albo","czy","i","lub","oraz"})
				)),
				// ustawiamy AfterConj na 0
				rlook(0,0,$AfterConj,equal({nom},{nom}))
			),
			and(
				// szukamy po prawej
				rlook(2,5,$AfterConj,and(
					inter(flex[$-1AfterConj],{conj}),
					in(orth[$-1AfterConj],{"albo","czy","i","lub","oraz"})
				)),
				// ustawiamy BeforeConj na 0
				llook(0,0,$BeforeConj,equal({nom},{nom}))
			)
		),
		only($BeforeConj,$AfterConj,$What,not(
			in({prep},flex[$What]),
			in(orth[$What],{"nie","Nie"})
		))
	)
)

//--------------------------------------------------------------

 : acc inst # <
   !casstd,
     //23
   !AfterPrepWeak,
     //24
   !PPAfterPrepWeak,
     //25
   !PPAfterPrepStrict
>

: acc loc # <
   !casstd,
   ?equal(cas[1],{acc})?equal(flex[0],{prep}),
   ?equal(cas[1],{loc})?equal(flex[0],{prep}),
   //?equal(orth[0],{"po"})?equal(flex[1],{adjp}),
   !AfterPrepWeak,
   !PPAfterPrepWeak,
   !PPAfterPrepStrict
>


: dat inst loc # <
   !casstd,
   !AfterPrepWeak,
   !AgrAcrossConjShort
>

: dat loc voc # <
   !casstd,
   !AfterPrepWeak,
   !AgrAcrossConjShort
>

: dat loc # <
   !casstd,
   !AfterPrepWeak
>

: gen acc loc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

 : gen acc loc voc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen acc voc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen acc # <
   !casstd,
   !AfterPrepWeak,
   !PPAfterPrepWeak,
   !PPAfterPrepStrict,
   //!AfterConjJako,
  !AfterPrepWeak,
  !AgrAcrossConjShort,
  !AgrAcrossConjLongAcc,
  !NieMa
>

: gen dat acc loc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa
>

: gen dat inst # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen dat inst loc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen dat loc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen dat loc voc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen dat voc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: gen dat # <
   !casstd,
   !AfterPrepWeak,
   !NieMa
>

: gen inst # <
   !casstd,
   isbig(orth[0]),
   !PPAfterPrepWeak,
   !PPAfterPrepStrict,
   !AfterPrepWeak,
   !NieMa
>

: gen loc voc # <
   !casstd,
   !AfterPrepWeak,
   !NieMa,
   !AgrAcrossConjShort
>

: inst loc # <
   !casstd,
   !AfterPrepWeak
>

: loc voc # <
   !casstd,
   !AfterPrepWeak
>

: nom acc loc voc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !AgrAcrossConjShort
>

: nom acc voc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !AgrAcrossConjShort
>

: nom acc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj
   //!NearestVerbLeft,
   //!NearestVerbRight
>

: nom dat acc loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !AgrAcrossConjShort
>

: nom dat acc inst loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !AgrAcrossConjShort
>

: nom dat acc loc voc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !AgrAcrossConjShort
>

: nom gen acc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen acc loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen acc voc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen dat acc loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen dat inst loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen dat acc inst loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen dat loc voc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

//: nom gen dat acc loc voc #
//   !casstd
//   !AfterPrepWeak
//   !PossibleSubj
//   !AgrAcrossConjShort

: nom gen dat acc inst loc voc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen inst # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa,
   !AgrAcrossConjShort
>

: nom gen loc # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa
>

: nom gen # <
   !casstd,
   !AfterPrepWeak,
   !PossibleSubj,
   !NieMa
>

: nom voc # <
   !casstd,
   !PossibleSubj
>
