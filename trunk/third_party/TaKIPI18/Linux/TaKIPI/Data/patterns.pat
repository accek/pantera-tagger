// atrybuty 0-22 to !std
!std (
   flex[-3], flex[-2], flex[-1],        flex[1], flex[2],
   cas[-3], cas[-2], cas[-1], cas[0], cas[1], cas[2],
   gnd[-3], gnd[-2], gnd[-1], gnd[0], gnd[1], gnd[2],
   nmb[-3], nmb[-2], nmb[-1], nmb[0], nmb[1], nmb[2]
)
!std_without_cas0 (
   flex[-3], flex[-2], flex[-1],        flex[1], flex[2],
   cas[-3], cas[-2], cas[-1],        cas[1], cas[2],
   gnd[-3], gnd[-2], gnd[-1], gnd[0], gnd[1], gnd[2],
   nmb[-3], nmb[-2], nmb[-1], nmb[0], nmb[1], nmb[2]
)

//sprawdza czy dla subst istnieje dopasowana fraza przymiotnikowa z prawej
!AdjPRight (
	or(
		and(
			inter(flex[1],{adj,ppas,pact}),
			agrpp(0,1,{cas,gnd,nmb},3)
		),
		and(
			rlook(2,end,$Adj,inter(flex[$Adj],{adj,ppas,pact})),
			agrpp(0,$Adj,{cas,gnd,nmb},3),
			only(1,$-1Adj,$Q,inter(flex[$Q],{adv,qub}))
		)
	)
)

!AdjPLeft (
	or(
		and(
			inter(flex[-1],{adj,ppas,pact}),
			agrpp(0,-1,{cas,gnd,nmb},3)
		),
		and(
			llook(-2,begin,$Adj,inter(flex[$Adj],{adj,ppas,pact})),
			agrpp(0,$Adj,{cas,gnd,nmb},3),
			only($+1Adj,-1,$Q,inter(flex[$Q],{adv,qub}))
		)
	)
)

//Sprawdza czy 0 moze byc podmiotem pod warunkiem posiadania {acc, nom}
!SubstCanIBeSubject (
	? or(
		and(
			rlook(1,end,$Fin,and(
								inter(flex[$Fin],{fin,praet,bedzie}),
								//inter(catflt(0,{nom,acc},{nmb}),catflt($Fin,{fin,praet,bedzie},{nmb}))
								inter(nmb[0],catflt($Fin,{fin,praet,bedzie},{nmb}))
							)),
			only(1,$Fin,$Cs,not(
								inter(cas[$Cs],{nom,acc})
					)),
			not(
				llook(-1,begin,$Sec,and(
										inter(cas[$Sec],{nom,acc}),
										//inter(catflt($Sec,{nom,acc},{nmb}),catflt($Fin,{fin,praet,bedzie},{nmb}))
										inter(nmb[0],catflt($Fin,{fin,praet,bedzie},{nmb}))
										))
			)
		),
		and(
			llook(-1,begin,$Fin,and(
								inter(flex[$Fin],{fin,praet,bedzie}),
								//inter(catflt(0,{nom,acc},{nmb}),catflt($Fin,{fin,praet,bedzie},{nmb}))
								inter(nmb[0],catflt($Fin,{fin,praet,bedzie},{nmb}))
							)),
			only($Fin,-1,$Cs,not(
								inter(cas[$Cs],{nom,acc})
					)),
			not(
				llook(-1,begin,$Sec,and(
										inter(cas[$Sec],{nom,acc}),
										//inter(catflt($Sec,{nom,acc},{nmb}),catflt($Fin,{fin,praet,bedzie},{nmb}))
										inter(nmb[0],catflt($Fin,{fin,praet,bedzie},{nmb}))
									))
			)
		)
	)?inter(cas[0],{nom,acc})
)

!NumPGenLeft (
	? or(
		and(
			inter(flex[-1],{num}),
			agrpp(0,-1,{pl,gnd,gen},3)
		),
		and(
			llook(-2,begin,$N,and(
					inter(flex[-1],{num}),
					agrpp(0,-1,{pl,gnd,gen},3)
			)),
			only($+1N,-1,$A,or(
					inter(flex[$A],{adv,qub}),
					and(
						inter(flex[$A],{adj,ppas,pact}),
						agrpp(0,$A,{pl,gnd,gen},3)
					)
			))
		)
	)?inter(cas[0],{gen})
)

!PPron3GenLeft (
	or(
		and(
			inter(flex[-1],{ppron3}),
			inter(cas[-1],{gen}),
			agrpp(0,-1,{nmb,gnd},2)
		),
		and(
			llook(-2,begin,$P,and(
				inter(flex[$P],{ppron3}),
				inter(cas[$P],{gen})
			)),
			only($+1P,-1,$A,or(
				inter(flex[$A],{adv,qub}),
				and(
					inter(flex[$A],{adj,ppas,pact}),
					agrpp(0,$A,{pl,gnd,gen},3)
				)
			))
		)
	)
)

//nie to fin|praet|bedzie|inf|pred|ger|conj|interp|prep
//poszukiwanie blisko sie
!SieNear (
	or(
		or(
			in(orth[1],{"się","Się"}),
			and(
				rlook(2,end,$S,in(orth[$S],{"się","Się"})),
				only(1,$-1S,$Q,inter(flex[$Q],{adv,qub,adj,ppas,pact,ppron3,ppron12}))
			)
		),
		or(
			in(orth[-1],{"się","Się"}),
			and(
				llook(-2,begin,$Sl,in(orth[$Sl],{"się","Się"})),
				only($+1Sl,-1,$Ql,inter(flex[$Ql],{adv,qub,adj,ppas,pact,ppron3,ppron12}))
			)
		)
	)
)

!FinAmIMainVerb (
	or(
		or(
			and(
				inter(flex[-1],{depr,ger,ign,subst}),
				//inter(catflt(0,{fin},{nmb}),catflt(-1,{depr,ger,ign,subst},{nmb}))
				inter(nmb[0],catflt(-1,{depr,ger,ign,subst},{nmb}))
			),
			and(
				inter(flex[-1],{ppron3,ppron12}),
				agrpp(0,-1,{nmb,per},2)
			),
			and(
				llook(-2,begin,$S,or(
						and(
							inter(flex[$S],{depr,ger,ign,subst}),
							//inter(catflt(0,{fin},{nmb}),catflt($S,{depr,ger,ign,subst},{nmb}))
							inter(nmb[0],catflt($S,{depr,ger,ign,subst},{nmb}))
						),
						and(
							inter(flex[$S],{ppron3,ppron12}),
							agrpp(0,$S,{nmb,per},2)
						)
				)),
				not(
						llook($-1S,begin,$F,and(
							inter(flex[$F],{fin,bedzie,praet}),
							inter(nmb[0],nmb[$F])
						))
				)
			)
		),
		or(
			and(
				inter(flex[1],{depr,ger,ign,subst}),
				//inter(catflt(0,{fin},{nmb}),catflt(1,{depr,ger,ign,subst},{nmb}))
				inter(nmb[0],catflt(1,{depr,ger,ign,subst},{nmb}))
			),
			and(
				inter(flex[1],{ppron3,ppron12}),
				agrpp(0,1,{nmb,per},2)
			),
			and(
				rlook(2,end,$S,or(
						and(
							inter(flex[$S],{depr,ger,ign,subst}),
							inter(nmb[0],catflt($S,{depr,ger,ign,subst},{nmb}))
						),
						and(
							inter(flex[$S],{ppron3,ppron12}),
							agrpp(0,$S,{nmb,per},2)
					)
				)),
				not(
					rlook($+1S,end,$F,and(
							inter(flex[$F],{fin,bedzie,praet}),
							inter(nmb[0],nmb[$F])
					))
				)
			)
		)
	)
)

!CasOfCoordLeft (
	? agrflt(-2,0,{cas,nmb,gnd},3,{cas}) ? and(
		inter(flex[-1],{conj}),
		in(orth[-1],{"albo","czy","i","lub","oraz"}),
		not(in(orth[-2],{"nie","Nie"})),
		not(in(orth[-0],{"nie","Nie"}))
	)
)

!CasAgrAcrossConjLongStrict (
	or(
		and(
			or(
				llook(0,-1,$C,and(
						inter(flex[$C],{conj}),
						in(orth[$C],{"albo","czy","i","lub","oraz"})
				)),
				and(
					llook(-2,begin,$C,and(
							equal(flex[$C],{conj}),
							in(orth[$C],{"albo","czy","i","lub","oraz"})
					)),
					only($+1C,-1,$OBC,or(
							not(equal(cas[$OBC],{none})),
							inter(flex[$OBC],{adv,qub})
					)),
					wagr($+1C,0,{cas,nmb,gen},3)
				)
			),
			or(
				and(
					llook($-1C,begin,$A, not(
							equal(cas[$A],{none})
					)),
					inter(cas[$A],cas[0]),
					only($A,$-1C,$OAC,or(
							not(equal(cas[$OAC],{none})),
							inter(flex[$OAC],{adv,qub})
					)),
					wagr($A,$-1C,{cas,nmb,gen},3)
				)
			)
		),
		and(
			or(
				rlook(0,1,$Cr,and(
						equal(flex[$Cr],{conj}),
						in(orth[$Cr],{"albo","czy","i","lub","oraz"})
				)),
				and(
					rlook(2,end,$Cr,and(
							equal(flex[$Cr],{conj}),
							in(orth[$Cr],{"albo","czy","i","lub","oraz"})
					)),
					only(1,$-1C,$OBCr,or(
							not(equal(cas[$OBCr],{none})),
							inter(flex[$OBCr],{adv,qub})
					)),
					wagr(0,$-1C,{cas,nmb,gen},3)
				)
			),
			or(
				and(
					rlook($+1C,end,$Ar,not(equal(cas[$Ar],{none}))),
					inter(cas[$A],cas[0]),
					only($+1C,$Ar,$OACr,or(
							not(equal(cas[$OACr],{none})),
							inter(flex[$OACr],{adv,qub})
					)),
					wagr($+1C,$Ar,{cas,nmb,gen},3)
				)
			)
		)
	)
)

adv praet : # <
   !std_without_cas0
   >

adj fin subst : # <
   !std
   >


qub subst : # <
   !std,
   agrpp(0,-1,{cas,gnd,nmb},3),
   ? agrpp(0,$Adj,{cas,gnd,nmb},3)?and(
                                  llook(-1,begin,$Adj,inter(flex[$Adj],{adj,ppas,pact})),
 	                          rlook($Adj,-1,$NotA,not(
				                         inter(flex[$NotA],{adj,ppas,pact})
						        )),
	                          only($NotA,-1,$Q,inter(flex[$Q],{adv,qub}))
                                 ),
   ? and(
      rlook(1,end,$Fin,inter(flex[$Fin],{fin,praet})),
      only(1,$Fin,$Cs,not(
                          inter(cas[$Cs],{nom,acc})
		      )),
      llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
      only($Sb,-1,$LF,not(
                          inter(flex[$LF],{fin,praet})
		      ))
   )?inter(cas[0],{nom,acc})
>

prep qub : # <
   flex[-3], flex[-2], flex[-1], flex[1], flex[2],
   cas[1],
   in(orth[1],{"tak","Tak"}),
   or(
       inter(cas[1],{gen,inst}),
       isbig(orth[1]),
       hasnum(orth[0])
   ),
   ? cas[$Cs]?rlook(1,end,$Cs,not(
                                equal(cas[$Cs],{none})
			    ))
>

adj ppas : # <
   !std,
   asp[0],
   //
   and(
     rlook(1,end,$NParg,and(
                            inter(flex[$NParg],{ger,subst}),
                            not(
			        agrpp(0,$NParg,{cas,nmb,gnd},3)
			    )
                         )),
     rlook($+1NParg,end,$NPdsc,and(
                                 inter(flex[$NPdsc],{ger,subst}),
                                 agrpp(0,$NPdsc,{cas,nmb,gnd},3)
                             )
     )
   ),
   llook(-1,begin,$NA,and(
                          inter(flex[$NA],{subst}),
                          agrpp(0,$NA,{cas,nmb,gnd},3)
                       )),
   //
   or(
      equal(base[-1],{"być"}),
      and(
         llook(-2,begin,$Byc,equal(base[$Byc],{"być"})),
         only($+1Byc,-1,$Q,inter(flex[$Q],{adv,qub}))
      )
   ),
   or(
      and(
          equal(flex[1],{conj}),
	  in(orth[1],{"albo","czy","i","lub","oraz"}),
	  inter(flex[2],{adj})
      ),
      and(
          equal(flex[-1],{conj}),
	  in(orth[-1],{"albo","czy","i","lub","oraz"}),
	  inter(flex[-2],{adj})
      )
    ),
    //!StrictNPAgr
    or(
       or(
         and(
	    in(flex[1],{ger,subst}),
	    agrpp(0,0,{cas,nmb,gnd},3)
		),
         and(
            rlook(1,end,$N,inter(flex[$N],{ger,subst})),
	    only(1,$-1N,$A,equal(flex[$A],{adj})),
	    wagr(0,$N,{cas,nmb,gnd},3)
		)
      ),
      or(
         and(
	    in(flex[-1],{ger,subst}),
	    agrpp(0,0,{cas,nmb,gnd},3)
	 ),
         and(
            llook(-1,begin,$N,inter(flex[$N],{ger,subst})),
	    only($+1N,-1,$A,equal(flex[$A],{adj})),
	    wagr($N,0,{cas,nmb,gnd},3)
	 )
      )
    )
>

adj subst : # <
   !std,
   equal(orth[0],{"tego"}),
   ? inter(gnd[1],catflt(0,{adj},{gnd}))?and(
                                        equal(flex[1],{subst}),
	                                agrpp(0,1,{cas,nmb,gnd},3)
	                               ),
   ? inter(gnd[-1],catflt(0,{adj},{gnd}))?and(
                                         equal(flex[-1],{subst}),
	                                 agrpp(0,-1,{cas,nmb,gnd},3)
	                                )
>

adv subst : # <
   !std,
   inter(cas[0],{nom,acc}), //23
   ?in({gen},cas[1])?in({subst},flex[1]), //24
   ?and( //25
      rlook(1,end,$Fin,inter(flex[$Fin],{fin,praet})),
      only( 1,$Fin,$Cs,not(
                           inter(cas[$Cs],{nom,acc})
		       )),
      llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
      only($Sb,-1,$LF,not(
                          inter(flex[$LF],{fin,praet})
		       ))
   )?inter(cas[0],{nom,acc}),
   in(orth[0],{"Dobrze","dobrze"}), //26
   in(orth[0],{"Jutro","jutro"}), //27
   or( //28
      in(orth[-1],{"Bardzo","bardzo"}),
      and(
        llook(-2,begin,$B,in(orth[-1],{"Bardzo","bardzo"})),
        only($+1B,-1,$Q,inter(flex[$Q],{adv,qub}))
      )
   ),
    //29
   !AdjPRight,
   //30
   !AdjPLeft
>

adj conj pred qub subst : # <
   !std,
   ?inter(cas[-1],cas[0])?in({prep},flex[-1]),
   ?equal(orth[-1],{","})?equal(flex[-1],{interp}),
   ?equal(orth[1],{","})?equal(flex[-1],{interp}),
   equal(orth[1],{"nie"}),
   ?agrpp(0,$Adj,{cas,gnd,nmb},3)?and(
        rlook(1,end,$Adj,inter(flex[$Adj],{adj,ppas,pact})),
	only(1,$Adj,$Q,inter(flex[$Q],{adv,qub,adja}))
       ),
   llook(-1,-4,$Ze,in(orth[$Ze],{"że","Że"})),
   and(
      rlook(1,end,$Fin,and(
                           equal(nmb[$Fin],{sg}),
                           inter(flex[$Fin],{fin,praet})
                         )),
      only(1,$-1Fin,$Cs,inter(flex[$Cs],{adv,qub}))
   )
>

fin qub : # <
   flex[-3], flex[-2], flex[-1],        flex[1], flex[2],
   nmb[-3], nmb[-2], nmb[-1], nmb[0], nmb[1], nmb[2],
   ?equal(orth[1],{"być"})?in(orth[0],{"Być","być"}),
   ?equal(orth[-1],{"być"})?in(orth[0],{"Być","być"}),
   or(
      and(
        rlook(1,end,$Inf,equal(flex[$Inf],{inf})),
        only(1,$-1Inf,$Q,equal(flex[$Q],{qub}))
      ),
      and(
        llook(-1,begin,$Inf,equal(flex[$Inf],{inf})),
        only($+1Inf,-1,$Q,equal(flex[$Q],{qub}))
      )
   ),
   in(orth[0],{"da","Da"}),
   ?equal(orth[1],{"się"})?in(orth[0],{"da","Da"})
>

// m.in. tyle
num subst: # <
  !std,
  in(orth[-1],{"nie","Nie","NIE","o","O"}),
  in(orth[-1],{"w","W","na","NA"})
  //in(base[1],{"że"})
>

prep subst: # <
  !std
>

adjp subst: # <
  !std
>

fin subst : # <
  !std,
  isbig(orth[0]),
  //23
  !AdjPRight,
  //24
  !AdjPLeft,
  //25 26
  !SubstCanIBeSubject,
  //27
  !NumPGenLeft,
  //28
  !PPron3GenLeft,
  //29
  !SieNear,
  //30
  !FinAmIMainVerb,
  //31
  //!CasAgrAcrossConjLongStrict
  !CasOfCoordLeft
>

fin ger subst: # <
  !std
>

adj conj subst: # <
  !std
>

adj winien : # <
  flex[-3], flex[-2], flex[-1], flex[1], flex[2],
  cas[-2], cas[-1], ?cas[0]?equal(flex[0],{adj}), cas[1], cas[2],
  rlook(1,end,$Inf,equal(flex[$Inf],{inf})),
  llook(-1,begin,$Byc,equal(base[$Byc],{"być"})),
  llook(-1,begin,$LSie,equal(base[$LSie],{"się"})),
  rlook(1,end,$RSie,equal(base[$RSie],{"się"}))
>

adj ppas subst : # <
   !std,
   and(
       rlook(1,end,$Byc,equal(base[$Byc],{"być"})),
       only(1,$-1Byc,$Q,inter(flex[$Q],{adv,qub,pcon,pant}))
       ),
   and(
       llook(-1,begin,$Byc,equal(base[$Byc],{"być"})),
       only($+1Byc,-1,$Q,inter(flex[$Q],{adv,qub}))
   ),
   !SubstCanIBeSubject,
   catflt(0,{subst},{nmb}),
   catflt(0,{subst},{gnd}),
   catflt(0,{subst},{cas})
>

adv fin subst: # <
  !std
>

fin ppron12 : # <
  !std
>

fin praet subst: # <
  !std
>

ger num subst: # <
  !std
>

adj praet subst: # <
  !std
>

inf subst: # <
  !std
>

adj adv impt: # <
  !std
>

fin impt subst: # <
  !std
>

adj inf: # <
  !std
>

imps subst: # <
  !std
>

adj adv : # <
  !std
>

adj pact subst: # <
  !std
>

adj impt subst: # <
  !std
>

adv ger subst: # <
  !std
>

depr fin: # <
  !std
>

fin pred subst: # <
  !std
>

pact qub : # <
  !std
>

adj depr subst: # <
  !std
>

adv conj subst : # <
  !std,
  equal(orth[-1],{","})
>

adj ppron12 : # <
  !std
>

adv qub	: # <
  !std
>

conj qub : # <
  flex[-3], flex[-2], flex[-1], flex[1], flex[2],
  ?equal(orth[-1],{","})?in(orth[0],{"by","By"}),
  isbig(orth[0]),
  inter(cas[-1],cas[1]),
  inter(flex[-1],flex[1]),
  ?equal(orth[-1],{","})?in(orth[0],{"kiedy","Kiedy"}),
  in(orth[0],{"kiedy","Kiedy"})
>

conj qub subst : # <
  flex[-3], flex[-2], flex[-1], flex[1], flex[2],
  equal(orth[-1],{","}),
  rlook(1,4,$Z,in(orth[$Z],{"że","Że"})),
  rlook(1,5,$Z,in(orth[$Z],{"jak","Jak"}))
>

conj prep : # <
  !std
>

adj num: # <
  !std
>

adj pred: # <
  !std
>

adj qub: # <
  !std,
  isbig(orth[0]),
  in(orth[0],{"te","Te"}),
  or(
    agrpp(0,1,{cas,nmb,gnd},3),
    agrpp(-1,0,{cas,nmb,gnd},3)
    ),
  //testowanie z przeskokiem przez qub
  ? and(
      inter(flex[1],{adv,qub}),
      agrpp(0,2,{cas,nmb,gnd},3)
    )?in(orth[0],{"te","Te","Ta","ta","me"})
>

conj impt subst: # <
  !std
>

conj ppron3 : # <
  !std
>

fin ppas: # <
  !std,
  catflt(0,{fin},{nmb}),
  catflt(0,{ppas},{nmb})
>

fin ppron3 : # <
  !std
>

fin qub subst: # <
  !std
>

impt inf subst: # <
  !std
>

impt pred subst: # <
  !std
>

inf pred subst: # <
  !std
>

ppron3 qub: # <
  !std,
  inter(orth[0],{"mu","Mu","jej","Jej"})
  //inter(orth[1],{"tyle","TYLE"})
>

impt pred qub : # <
   !std
>

praet qub : # <
   !std
>

pcon subst : # <
   !std
>

conj prep qub subst : # <
//klasa co
   !std,
   ?inter(cas[-1],cas[0])?in({prep},flex[-1]),
   ?equal(orth[-1],{","})?equal(flex[-1],{interp}),
   ?equal(orth[1],{","})?equal(flex[-1],{interp}),
   equal(orth[1],{"nie"}),
   ?agrpp(0,$Adj,{cas,gnd,nmb},3)?and(
        rlook(1,end,$Adj,inter(flex[$Adj],{adj,ppas,pact})),
	only(1,$Adj,$Q,inter(flex[$Q],{adv,qub,adja}))
       ),
   llook(-1,-4,$Ze,in(orth[$Ze],{"że","Że"})),
   and(
      rlook(1,end,$Fin,and(
                           equal(nmb[$Fin],{sg}),
                           inter(flex[$Fin],{fin,praet})
                         )),
      only(1,$-1Fin,$Cs,inter(flex[$Cs],{adv,qub}))
   )
>

pred adv : # <
    !std
>

prep adv : # <
   !std
>

impt qub prep : # <
   !std
>

adv impt qub : # <
   !std
>
