  INSERT INTO
    Canal (intEstudoId, intMedidorId, intCanalId, fltSigma, intTipoCanalId)
  SELECT [p_EstudoIdNovo], intMedidorId, intCanalId, fltSigma, intTipoCanalId
  FROM Canal WHERE intEstudoId = [p_EstudoIdOrg];
