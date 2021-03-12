    UPDATE
      (SELECT * FROM Suprimento) sel
    SET
      sel.intTipoSuprId = [p_TipoSuprId],
      sel.fltVnom = [p_Vnom],
      sel.fltSmax = [p_sMax],
      sel.fltPcc3F = [p_Pcc3F],
      sel.fltQcc3F = [p_Qcc3F],
      sel.fltPccFT = [p_PccFT],
      sel.fltQccFT = [p_QccFT],
      sel.fltZ0R = [p_Z0R],
      sel.fltZ0X = [p_Z0X],
      sel.fltZ1R = [p_Z1R],
      sel.fltZ1X = [p_Z1X]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intSuprimentoId = [p_SuprimentoId];
