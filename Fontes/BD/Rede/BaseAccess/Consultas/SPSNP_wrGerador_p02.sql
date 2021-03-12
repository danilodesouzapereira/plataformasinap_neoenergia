    UPDATE
      (SELECT * FROM Gerador) sel
    SET
      sel.intTipoLigacaoId = [p_TipoLigacaoId],
      sel.intTipoSuprId = [p_TipoSuprId],
      sel.intTipoGerId =  [p_TipoGerId],
      sel.fltVnom = [p_Vnom],
      sel.fltSnom = [p_Snom],
      sel.fltSmax = [p_sMax],
      sel.fltFatorPotMin = [p_FatorPotMin],
      sel.fltZ0R = [p_Z0R],
      sel.fltZ0X = [p_Z0X],
      sel.fltZ1R = [p_Z1R],
      sel.fltZ1X = [p_Z1X]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intGeradorId = [p_GeradorId];
