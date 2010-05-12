// TODO poprawić formatowanie
!nmbstd (
   flex[-3], flex[-2], flex[-1], flex[0], flex[1], flex[2],
   cas[-3], cas[-2], cas[-1], cas[0], cas[1], cas[2],
   gnd[-3], gnd[-2], gnd[-1],        gnd[1], gnd[2],
   nmb[-3], nmb[-2], nmb[-1],        nmb[1], nmb[2]
)
   
!NGndWide (
   ?gnd[$G]?or(
              and(
                  in(flex[0],{num,adj,pact,ppas}),
                  rlook(1,1,$G,equal(flex[$G],{subst})),
	          agrpp(0,$G,{cas,nmb,gnd},3)
              ),
	      and(
                in(flex[0],{num,adj,pact,ppas}),
                rlook(2,end,$G,equal(flex[$G],{subst})),
                only(1,$-1G,$A,in(flex[$A],{num,adj,pact,ppas})),
                wagr(0,$G,{cas,nmb,gnd},3)
	      ),
	      and(
                 in(flex[0],{adj,pact,ppas}),
                 llook(-1,-1,$G,equal(flex[$G],{subst})),
                 agrpp(0,$G,{cas,nmb,gnd},3)
	      ),
              and(
                in(flex[0],{adj,pact,ppas}),
                llook(-2,begin,$G,equal(flex[$G],{subst})),
                only($+1G,-1,$A,in(flex[$A],{num,adj,pact,ppas})),
                wagr($G,0,{cas,nmb,gnd},3)
	      )
            )
)

// szuka rzeczownika, który może być poprzednikiem ppron3
!AnaphorGnd (
	?gnd[$Ana]?and(
		inter(flex[0],{ppron3}),
		llook(-1,begin,$Ana,
			and(
				inter(flex[$Ana],{subst}),
				not(inter(base[$Ana],{"to","co","nic"})),
				agrpp(0,$Ana,{nmb,gnd},2)
			)
		)
	)
)

// zwraca zbiór liczb spełniający uzgodnienie wokół koordynacji
!CoordNumber (
	? agrflt($BeforeConj,$AfterConj,{cas,nmb,gnd},3,{nmb}) ? and(
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

// zwraca zbiór liczb spełniający uzgodnienie wokół koordynacji
!CoordGender (
	? agrflt($BeforeConj,$AfterConj,{cas,nmb,gnd},3,{gnd}) ? and(
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

: sg pl m1 # <
   !nmbstd,
   ?nmb[$FLNum]?llook(-1,begin,$FLNum,inter(nmb[$FLNum],{sg,pl})),
   ?nmb[$FLNum]?rlook(1,end,$FRNum,inter(nmb[$FRNum],{sg,pl})),
   ?in({m1},gnd[$FLGnd])?llook(-1,begin,$FLGnd,not(
                                                  equal(gnd[$FLGnd],{none})
					       )),
   ?in({m1},gnd[$FRGnd])?rlook(1,end,$FRGnd,not(
                                               equal(gnd[$FRGnd],{none})
					   )),
   and(
     llook(-1,begin,$A,inter(flex[$A],{adj})),
     agrpp($A,0,{cas,m1,sg},3)
   ),
   and(
     llook(-1,begin,$A,inter(flex[$A],{adj})),
     agrpp($A,0,{cas,m1,pl},3)
   ),
   //SignSubj --- dodac w prawo
   ? nmb[$Fin] ? and(
                inter(cas[0],{nom,acc}),
                rlook(1,end,$Fin,inter(flex[$Fin],{bedzie,fin,praet})),
                only(1,$Fin,$Cs,not(
                                  inter(cas[$Cs],{nom,acc})
		                )),
		or(
                  not(
		      llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc}))
		  ),
		  and(
		    llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
                    llook(-1,$Sb,$LF,inter(flex[$LF],{bedzie,fin,praet}))
		  )
		)
    ),
   //PluralCoord
   and(
    llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
    llook(-1,$Sb,$C,equal(flex[$C],{conj})),
    only($Sb,$C,$LF,not(
                          inter(flex[$LF],{bedzie,fin,praet})
		      ))
   ),
   !CoordNumber
>

: sg m1 m2 # <
   !nmbstd,
   isbig(orth[0]),
   in({"bóg"},base[0]),
   !CoordGender
>

: pl m1 m2 # <
   flex[-3], flex[-2], flex[-1], flex[0], flex[1], flex[2],
   cas[-3], cas[-2], cas[-1], cas[0], cas[1], cas[2],
   ?agrpp(0,$AR1,{cas,m1,pl},3)?rlook(1,end,$Adj,inter(flex[$AR1],{adj,ppas,pact})),
   ?agrpp(0,$AR2,{cas,m2,pl},3)?rlook(1,end,$AR2,inter(flex[$AR2],{adj,ppas,pact})),
   ?agrpp(0,$AL1,{cas,m1,pl},3)?llook(-1,begin,$AL1,inter(flex[$AL1],{adj,ppas,pact})),
   ?agrpp(0,$AL2,{cas,m2,pl},3)?llook(-1,begin,$AL2,inter(flex[$AL2],{adj,ppas,pact})),
   !CoordGender
>

: sg pl m3 # <
   !nmbstd,
   !CoordNumber
>

: sg m1 m3 # <
   flex[-3], flex[-2], flex[-1], flex[0], flex[1], flex[2],
   cas[-3], cas[-2], cas[-1], cas[0], cas[1], cas[2],
   isbig(orth[0]),
   ?agrpp(0,$AR1,{cas,m1,sg},3)?rlook(1,end,$AR1,inter(flex[$AR1],{adj,ppas,pact})),
   ?agrpp(0,$AR2,{cas,m3,sg},3)?rlook(1,end,$AR2,inter(flex[$AR2],{adj,ppas,pact})),
   ?agrpp(0,$AL1,{cas,m1,sg},3)?llook(-1,begin,$AL1,inter(flex[$AL1],{adj,ppas,pact})),
   ?agrpp(0,$AL2,{cas,m3,sg},3)?llook(-1,begin,$AL2,inter(flex[$AL2],{adj,ppas,pact})),
   !CoordGender
>

: pl m1 m3 # <
   !nmbstd,
   in({gen},cas[-1]),
   in({gen},cas[1]),
   isbig(orth[0]),
   !CoordGender
>

: sg m2 m3 # <
   !nmbstd,
   !CoordGender
>

: pl m2 m3 # <
   !nmbstd,
   !CoordGender
>

: sg m1 m2 m3 # <
	!nmbstd,
	?gnd[$Sb]?or(
		and(
			llook(0,-1,$Sb,inter(cas[$Sb],{nom,acc})),
			agrpp($Sb,0,{sg,m1,m2,m3},2)
		),
		and(
			llook(-2,begin,$Sb,inter(cas[$Sb],{nom,acc})),
			agrpp($Sb,0,{sg,m1,m2,m3},2),
			only($+1Sb,-1,$O,not(
				in(flex[$O],{bedzie,fin,praet})
			))
		)
	),
	?gnd[$Sbr]?or(
		and(
			rlook(0,1,$Sbr,inter(cas[$Sbr],{nom,acc})),
			agrpp($Sbr,0,{sg,m1,m2,m3},2)
		),
		and(
			rlook(2,end,$Sbr,inter(cas[$Sbr],{nom,acc})),
			agrpp($Sbr,0,{sg,m1,m2,m3},2),
			only($-1Sbr,-1,$Or,not(
				in(flex[$Or],{bedzie,fin,praet})
			))
		)
	),
	? nmb[$Fin] ? and(
		inter(cas[0],{nom,acc}),
		rlook(1,end,$Fin,inter(flex[$Fin],{fin,praet})),
		only(1,$Fin,$Cs,not(
			inter(cas[$Cs],{nom,acc})
		)),
		llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
		only($Sb,-1,$LF,not(
			inter(flex[$LF],{fin,praet})
		))
	),
	?gnd[1]?and(
		in(flex[0],{adj,pact,ppas}),
		equal(flex[1],{subst}),
		agrpp(0,1,{cas,nmb,gnd},3)
	),
	?gnd[-1]?and(
		in(flex[0],{adj,pact,ppas}),
		equal(flex[-1],{subst}),
		agrpp(0,-1,{cas,nmb,gnd},3)
	),
	!AnaphorGnd,
	!CoordGender
>

: pl m1 m2 m3 # <
   !nmbstd,
   !AnaphorGnd,
   !CoordGender
>

: sg pl m1 m2 m3 # <
   !nmbstd,
   ?cas[-1]?in(flex[-1],{ger,subst}),
   ?cas[0]?equal(flex[0],{subst}),
   ?gnd[$Sb]?or(
            and(
	        llook(0,-1,$Sb,equal(flex[$Sb],{subst})),
                agrpp($Sb,0,{cas,nmb,m1,m2,m3},3)
	    ),
	    and(
              llook(-2,begin,$Sb,equal(flex[$Sb],{subst})),
	      agrpp($Sb,0,{cas,nmb,m1,m2,m3},3),
	      only($+1Sb,1,$PL,not(
	                           equal(flex[$PL],{prep})
				))
            )
           ),
   ?gnd[$Sbr]?or(
            and(
	        rlook(0,1,$Sbr,equal(flex[$Sbr],{subst})),
                agrpp($Sbr,0,{cas,nmb,m1,m2,m3},3)
	    ),
	    and(
              rlook(2,end,$Sbr,equal(flex[$Sbr],{subst})),
	      agrpp($Sbr,0,{cas,nmb,m1,m2,m3},3),
	      only($-1Sbr,1,$PR,not(
	                            equal(flex[$PR],{prep})
				))
            )
           ),
   ? nmb[$Fin] ? and(
      inter(cas[0],{nom,acc}),
      rlook(1,end,$Fin,inter(flex[$Fin],{fin,praet})),
      only(1,$Fin,$Cs,not(
                          inter(cas[$Cs],{nom,acc})
		      )),
      llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
      only($Sb,-1,$LF,not(
                          inter(flex[$LF],{fin,praet})
		      ))
   ),
   ?gnd[1]?and(
              in(flex[0],{adj,pact,ppas}),
              equal(flex[1],{subst}),
	      agrpp(0,1,{cas,nmb,gnd},3)
	  ),
   ?gnd[-1]?and(
               in(flex[0],{adj,pact,ppas}),
               equal(flex[-1],{subst}),
	       agrpp(0,-1,{cas,nmb,gnd},3)
	   ),
	!AnaphorGnd,
	!CoordGender,
	!CoordNumber
>

: sg pl f # <
   !nmbstd,
   ?cas[$P]?or(
	    llook(0,-1,$P,equal(flex[$P],{prep})),
	    and(
              llook(end,-2,$P,equal(flex[$P],{prep})),
	      only($+1P,-1,$O,in(flex[$O],{adj,ppas,pact})),
	      wagr($+1P,0,{cas,nmb,gnd},3)
	    )
           ),
   ?catflt($AL,{subst,adj},{nmb,f})?or(
                            and(
			       llook(0,-1,$AL,in(flex[$AL],{adj,ppas,pact})),
                               agrpp($AL,0,{cas,nmb,f},3)
                            ),
                            and(
                               llook(-2,begin,$AL,in(flex[$AL],{adj,ppas,pact})),
                               agrpp($AL,0,{cas,nmb,f},3),
                               only($AL,-1,$OL,not(
			                           in(flex[$OL],{subst,ger,num})
						))
	                    )
			 ),
   ?catflt($AR,{subst,adj},{nmb,f})?or(
                             and(
			       rlook(0,1,$AR,in(flex[$AR],{adj,ppas,pact})),
                               agrpp($AR,0,{cas,nmb,f},3)
			     ),
                             and(
                                 rlook(2,end,$AR,in(flex[$AR],{adj,ppas,pact})),
	                         agrpp($AR,0,{cas,nmb,f},3),
                                 only($AR,-1,$ORL,not(
				                      in(flex[$ORL],{subst,ger,num})
						  ))
	                     )
			   ),
	!CoordNumber
>

: sg m1 f # <
   !nmbstd,
   !CoordGender
>

: pl m1 f # <
   !nmbstd,
   !CoordGender
>

: sg pl m1 f # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: sg m2 f # <
   !nmbstd,
   !CoordGender
>

: sg pl m2 f # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: sg m3 f # <
   !nmbstd,
   !CoordGender
>

: pl m3 f # <
   !nmbstd,
   !CoordGender
>

: sg pl m3 f # <
   !nmbstd,
   in(orth[0],{"bez","Bez"}),
   ?cas[$P]?or(
	    llook(0,-1,$P,equal(flex[$P],{prep})),
	    and(
              llook(-2,end,$P,equal(flex[$P],{prep})),
	      only($+1P,-1,$O,in(flex[$O],{adj,ppas,pact})),
	      wagr($+1P,0,{cas,nmb,gnd},3)
	    )
           ),
   catflt(0,{subst,adj},{gnd,gen}),
   catflt(0,{subst,adj},{gnd,acc,nom}),
   catflt(0,{subst,adj},{gnd,loc,dat,inst}),
   catflt(0,{subst,adj},{nmb,gen}),
   catflt(0,{subst,adj},{nmb,nom,acc}),
   catflt(0,{subst,adj},{nmb,loc,dat,inst}),
   ?catflt($AL,{subst,adj},{nmb,m3,f})?or(
                            and(
			       llook(0,-1,$AL,in(flex[$AL],{adj,ppas,pact})),
                               agrpp($AL,0,{cas,nmb,m3,f},3)
                            ),
                            and(
                               llook(-2,begin,$AL,in(flex[$AL],{adj,ppas,pact})),
                               agrpp($AL,0,{cas,nmb,m3,f},3),
                               only($AL,-1,$OL,not(
			                           in(flex[$OL],{subst,ger,num})
					        ))
	                    )
			 ),
   ?catflt($AR,{subst,adj},{nmb,m3,f})?or(
                             and(
			       rlook(0,1,$AR,in(flex[$AR],{adj,ppas,pact})),
                               agrpp($AR,0,{cas,nmb,m3,f},3)
			     ),
                             and(
                                 rlook(2,end,$AR,in(flex[$AR],{adj,ppas,pact})),
	                         agrpp($AR,0,{cas,nmb,m3,f},3),
                                 only($AR,-1,$ORL,not(
				                      in(flex[$ORL],{subst,ger,num})
						  ))
	                     )
			   ),
   !CoordGender,
   !CoordNumber
>

: sg pl m1 m3 f # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: sg m1 m2 m3 f # <
   !nmbstd,
   !AnaphorGnd,
   !CoordGender
>

: pl m1 m2 m3 f # <
   !nmbstd,
   !AnaphorGnd,
   !CoordGender
>

: sg pl m1 m2 m3 f # <
   !nmbstd,
   isbig(orth[0]),
   in(orth[0],{"taki","Taki"}),
   ?nmb[$F]?or(
              rlook(0,1,$F,in(flex[$F],{bedzie,fin,praet})),
	      and(
	        rlook(2,end,$F,in(flex[$F],{bedzie,fin,praet})),
                only(1,$-1F,$Cs,not(
		                    inter(cas[$Cs],{nom,acc})
				))
	      )
	   ),
   ?cas[$P]?or(
	    llook(0,-1,$P,equal(flex[$P],{prep})),
	    and(
              llook(end,-2,$P,equal(flex[$P],{prep})),
	      only($+1P,-1,$O,in(flex[$O],{adj,ppas,pact})),
	      wagr($+1P,0,{cas,nmb,gnd},3)
	    )
           ),
   catflt(0,{subst,adj},{gnd,acc}),
   catflt(0,{subst,adj},{gnd,nom}),
   catflt(0,{subst,adj},{gnd,loc,dat,inst}),
   catflt(0,{subst,adj},{nmb,acc}),
   catflt(0,{subst,adj},{nmb,nom}),
   catflt(0,{subst,adj},{nmb,loc,dat,inst}),
   ?catflt($AL,{subst,adj},{nmb,m1,m2,m3,f})?or(
                            and(
			       llook(0,-1,$AL,in(flex[$AL],{adj,ppas,pact})),
                               agrpp($AL,0,{cas,nmb,m1,m2,m3,f},3)
                            ),
                            and(
                               llook(begin,-2,$AL,in(flex[$AL],{adj,ppas,pact})),
                               agrpp($AL,0,{cas,nmb,m1,m2,m3,f},3),
                               only($AL,-1,$OL,not(
			                           in(flex[$OL],{subst,ger,num})
					       ))
	                    )
			 ),
   ?catflt($AR,{subst,adj},{nmb,m1,m2,m3,f})?or(
                             and(
			       rlook(0,1,$AR,in(flex[$AR],{adj,ppas,pact})),
                               agrpp($AR,0,{cas,nmb,m1,m2,m3,f},3)
			     ),
                             and(
                                 rlook(2,end,$AR,in(flex[$AR],{adj,ppas,pact})),
	                         agrpp($AR,0,{cas,nmb,m1,m2,m3,f},3),
                                 only($AR,-1,$ORL,not(
				                      in(flex[$ORL],{subst,ger,num})
						   ))
	                     )
			   ),
	!AnaphorGnd,
	!CoordNumber,
	!CoordGender
>

: sg pl n # <
   !nmbstd,
   ?and(
       in({gen},cas[-1]),
       in({gen},cas[0])
   )?only(-1,0,$P,equal(flex[$P],{subst})),
   !CoordNumber
>

: sg m1 n # <
   !nmbstd,
   ?and(
       in({gen},cas[0]),
       in({gen},cas[-1])
   )?and(
     in(flex[0],{subst,ger,num}),
     in(flex[-1],{subst,ger,num})
   ),
   ?and(
       in({gen},cas[0]),
       in({gen},cas[1])
   )?and(
     in(flex[0],{subst,ger,num}),
     in(flex[1],{subst,ger,num})
   ),
   ?cas[$P]?or(
	    llook(0,-1,$P,equal(flex[$P],{prep})),
	    and(
              llook(end,-2,$P,equal(flex[$P],{prep})),
	      only($+1P,-1,$O,in(flex[$O],{adj,ppas,pact})),
	      wagr($+1P,0,{cas,nmb,gnd},3)
	    )
           ),
   catflt(0,{subst,adj},{sg,m1,loc,voc}),
   catflt(0,{subst,adj},{sg,n,nom,acc,dat,loc}),
   !CoordGender
>

: pl m1 n # <
   !nmbstd,
   ?agrpp(-1,0,{cas,pl,m1},3)?in(flex[-1],{adj,ppas,pact}),
   ?agrpp(0,1,{cas,pl,m1},3)?in(flex[1],{adj,ppas,pact}),
   ?agrpp($AL,0,{cas,pl,m1},3)?and(
             llook(-2,begin,$AL,in(flex[$AL],{adj,ppas,pact})),
             only($AL,-1,$OL,not(
	                         in(flex[$OL],{subst,ger,num})
			     ))
	     ),
   ?agrpp($AR,0,{cas,pl,m1},3)?and(
             rlook(2,end,$AR,in(flex[$AR],{adj,ppas,pact})),
             only($AR,-1,$ORL,not(
	                          in(flex[$ORL],{subst,ger,num})
			      ))
	     ),
   ?and(
       in({gen},cas[0]),
       in({gen},cas[-1])
   )?and(
     in(flex[0],{subst,ger,num}),
     in(flex[-1],{subst,ger,num})
   ),
   ?and(
       in({gen},cas[0]),
       in({gen},cas[1])
   )?and(
     in(flex[0],{subst,ger,num}),
     in(flex[1],{subst,ger,num})
   ),
   !CoordGender
>

: sg pl m1 n # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: pl m3 n # <
   !nmbstd,
   !CoordGender
>

: sg pl m3 n # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: sg m1 m3 n # <
   !nmbstd,
   !CoordGender
>

: pl m1 m3 n # <
   !nmbstd,
   !CoordGender
>

: sg pl m1 m3 n # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: pl m2 m3 n # <
   !nmbstd,
   !CoordGender
>

: sg m1 m2 m3 n # <
   !nmbstd,
   ?gnd[1]?and(
              in(flex[0],{adj,pact,ppas}),
              equal(flex[1],{subst}),
	      agrpp(0,1,{cas,nmb,gnd},3)
	  ),
   ?gnd[-1]?and(
               in(flex[0],{adj,pact,ppas}),
               equal(flex[-1],{subst}),
	       agrpp(0,-1,{cas,nmb,gnd},3)
	   ),
	!AnaphorGnd,
	!CoordGender
>

: pl m1 m2 m3 n # <
   !nmbstd,
   !AnaphorGnd
>

: sg pl m1 m2 m3 n # <
   !nmbstd,
   !AnaphorGnd,
   !CoordGender,
   !CoordNumber
>

: sg pl f n # <
   !nmbstd,
   !CoordGender,
   !CoordNumber
>

: sg pl m1 m2 m3 n f # <
   !nmbstd,
   isbig(orth[0]),
   ?gnd[$Sb]?or(
            and(
	        llook(0,-1,$Sb,equal(flex[$Sb],{subst})),
                agrpp($Sb,0,{cas,nmb,gnd},3)
	    ),
	    and(
              llook(-2,begin,$Sb,equal(flex[$Sb],{subst})),
	      agrpp($Sb,0,{cas,nmb,gnd},3),
	      only($+1Sb,1,$PL,not(
	                           equal(flex[$PL],{prep})
				))
            )
           ),
   ?gnd[$Sbr]?or(
            and(
	        rlook(0,1,$Sbr,equal(flex[$Sbr],{subst})),
                agrpp($Sbr,0,{cas,nmb,gnd},3)
	    ),
	    and(
              rlook(2,end,$Sbr,equal(flex[$Sbr],{subst})),
	      agrpp($Sbr,0,{cas,nmb,gnd},3),
	      only($-1Sbr,1,$PR,not(
	                            equal(flex[$PR],{prep})
				))
            )
           ),
   ? nmb[$Fin] ? and(
      inter(cas[0],{nom,acc}),
      rlook(1,end,$Fin,inter(flex[$Fin],{fin,praet})),
      only(1,$Fin,$Cs,not(
                          inter(cas[$Cs],{nom,acc})
		      )),
      llook(-1,begin,$Sb,inter(cas[$Sb],{nom,acc})),
      only($Sb,-1,$LF,not(
                          inter(flex[$LF],{fin,praet})
		       ))
   ),
   ?gnd[$Z]?and(
             in(orth[0],{"Były","były"}),
             equal(orth[0],{"to"}),
	     rlook(2,end,$Z,in(flex[$Z],{ger,subst})
           )),
   !AnaphorGnd
>

: pl m1 f n # <
   !nmbstd
>

: sg pl m1 f n # <
   !nmbstd
>

: sg m3 f n # <
   !nmbstd
>

: pl m3 f n # <
   !nmbstd
>

: sg pl m1 m3 f n # <
   !nmbstd
>

: pl m2 m3 f n # <
   !nmbstd,
   !NGndWide
>

// m.in. tyle
: sg pl m2 m3 f n # <
	!nmbstd,
	?gnd[1]?and(
		in(flex[0],{adj,pact,ppas}),
		equal(flex[1],{subst}),
		agrpp(0,1,{cas,nmb,gnd},3)
	),
	?gnd[-1]?and(
		in(flex[0],{adj,pact,ppas}),
		equal(flex[-1],{subst}),
		agrpp(0,-1,{cas,nmb,gnd},3)
	)
>

: sg m1 m2 m3 f n # <
   !nmbstd,
   !AnaphorGnd
>

: pl m1 m2 m3 f n # <
   !nmbstd,
   ?gnd[1]?and(
              in(flex[0],{adj,pact,ppas}),
              equal(flex[1],{subst}),
	      agrpp(0,1,{cas,nmb,gnd},3)
	  ),
   ?gnd[-1]?and(
               in(flex[0],{adj,pact,ppas}),
               equal(flex[-1],{subst}),
	       agrpp(0,-1,{cas,nmb,gnd},3)
	   ),
   !AnaphorGnd
>
