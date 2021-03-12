  DELETE FROM
    Capacitor
  WHERE
    intEstudoId = [p_EstudoId] AND
    intCapacitorId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN Capacitor EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intCapacitorId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 5
    );

