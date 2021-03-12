  DELETE FROM
    RRTrafo
  WHERE
    intEstudoId = [p_EstudoId] AND
    intRRTrafoId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN RRTrafo EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intRRTrafoId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 24
    );
