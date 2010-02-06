m4_divert(`-1')
# forloop(var, from, to, stmt) - improved version:
#   works even if VAR is not a strict macro name
#   performs sanity check that FROM is larger than TO
#   allows complex numerical expressions in TO and FROM
m4_define(`forloop', `m4_ifelse(m4_eval(`($2) <= ($3)'), `1',
  `m4_pushdef(`$1')_$0(`$1', m4_eval(`$2'),
    m4_eval(`$3'), `$4')m4_popdef(`$1')')')
m4_define(`_forloop',
  `m4_define(`$1', `$2')$4`'m4_ifelse(`$2', `$3', `',
    `$0(`$1', m4_incr(`$2'), `$3', `$4')')')
m4_divert`'m4_dnl
