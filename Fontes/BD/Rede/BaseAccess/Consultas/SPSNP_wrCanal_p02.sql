    UPDATE
      (SELECT * FROM Canal) sel
    SET
      sel.fltSigma = [p_Sigma],
      sel.intTipoCanalId = [p_TipoCanalId],
      sel.intEquipId = [p_EquipId],
      sel.intTipoFaseId = [p_TipoFaseId]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intMedidorId = [p_MedidorId] AND
      sel.intCanalId = [p_CanalId];
