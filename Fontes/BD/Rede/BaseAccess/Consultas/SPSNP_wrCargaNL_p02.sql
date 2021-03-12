    UPDATE
      (SELECT * FROM CargaNL) sel
    SET
      sel.fltFatorPot = [p_FatorPot],
      sel.bolIndutivo = [p_Indutivo],
      sel.bolkVA      = [p_kVA],
      sel.fltSNomA    = [p_SNomA],
      sel.fltSNomB    = [p_SNomB],
      sel.fltSNomC    = [p_SNomC],
      sel.fltAngA     = [p_AngA],
      sel.fltAngB     = [p_AngB],
      sel.fltAngC     = [p_AngC]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intCargaNLId = [p_CargaNLId] AND
      sel.intOrdemId = [p_OrdemId];
