  DELETE FROM
    CapacitorSerie
  WHERE
    intEstudoId = [p_EstudoId] AND
    intCapacitorSerieId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN CapacitorSerie EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intCapacitorSerieId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 6
    );
