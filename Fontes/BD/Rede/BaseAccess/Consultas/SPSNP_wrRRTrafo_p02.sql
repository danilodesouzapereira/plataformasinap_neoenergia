    UPDATE
      (SELECT * FROM RRTrafo) sel
    SET
      sel.fltZ0R              = [p_Z0R],
      sel.fltZ0X              = [p_Z0X],
      sel.fltZ1R              = [p_Z1R],
      sel.fltZ1X              = [p_Z1X],
      sel.blbMatY             = [p_MatY],
      sel.intPriBarraId       = [p_Barra1Id],
      sel.intPriTipoLigacaoId = [p_Enrol1_TipoLigacaoId],
      sel.intPriTipoFaseId    = [p_Enrol1_TipoFaseId],
      sel.fltPriVnom          = [p_Enrol1_Vnom],
      sel.fltPriSnom          = [p_Enrol1_Snom],
      sel.fltPriZterraR       = [p_Enrol1_ZterraR],
      sel.fltPriZterraX       = [p_Enrol1_ZterraX],
      sel.intSecBarraId       = [p_Barra2Id],
      sel.intSecTipoLigacaoId = [p_Enrol2_TipoLigacaoId],
      sel.intSecTipoFaseId    = [p_Enrol2_TipoFaseId],
      sel.fltSecVnom          = [p_Enrol2_Vnom],
      sel.fltSecSnom          = [p_Enrol2_Snom],
      sel.fltSecZterraR       = [p_Enrol2_ZterraR],
      sel.fltSecZterraX       = [p_Enrol2_ZterraX],
      sel.fltRotFase          = [p_RotFase], 
      sel.intModeloEqv        = [p_ModeloEqv]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intMRedeId = [p_MRedeId] AND
      sel.intRRTrafoId = [p_RRTrafoId] AND
      sel.intCorteId = [p_CorteId];
