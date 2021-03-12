    UPDATE
      (SELECT * FROM RRSuprimento) sel
    SET
      sel.intTipoSuprId = [p_TipoSuprId],
      sel.fltVnom = [p_Vnom],
      sel.fltSmax = [p_Smax],
      sel.fltPcc3F = [p_Pcc3F],
      sel.fltQcc3F = [p_Qcc3F],
      sel.fltPccFT = [p_PccFT],
      sel.fltQccFT = [p_QccFT],
      sel.fltZ0REqv = [p_Z0REqv],
      sel.fltZ0XEqv = [p_Z0XEqv],
      sel.fltZ1REqv = [p_Z1REqv],
      sel.fltZ1XEqv = [p_Z1XEqv]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intMRedeId = [p_MRedeId] AND
      sel.intRRSuprimentoId = [p_RRSuprimentoId] AND
      sel.intCorteId = [p_CorteId];
