  DELETE FROM
    RRYRef
  WHERE
    intEstudoId = [p_EstudoId] AND
    intRRYRefId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN RRYRef EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intRRYRefId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 29
    );
