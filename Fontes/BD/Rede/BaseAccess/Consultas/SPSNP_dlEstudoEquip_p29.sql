  DELETE FROM
    RedeDominio
  WHERE
    intEstudoId = [p_EstudoId] AND
    intRedeId IN
    (
    SELECT
      Equipamento.intEquipId
    FROM
      Equipamento INNER JOIN Rede EqpAux ON
        Equipamento.intEstudoId = EqpAux.intEstudoId AND
        Equipamento.intEquipId = EqpAux.intRedeId
    WHERE
      Equipamento.intEstudoId = [p_EstudoId] AND
      (( [p_RedeInclusive] = 1) OR (Equipamento.bolAlteradoObra = 1)) AND
      Equipamento.intTipoEquipId = 19
    );
